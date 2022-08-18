#include "view.h"

#include <math.h>
#include <stdio.h>

#include "util/vector.h"

View *view_create(
    SDL_Window *window,
    ViewPort *port,
    void(*draw_function)(View*, void*),
    void *data
) {
    // Prevent null pointers.
    if (!window || !port || !draw_function)
        return NULL;

    // Allocate memory for the View data structure.
    View *view = malloc(sizeof(View));
    if (!view) {
        return NULL;
    }

    // Create the renderer.
    view->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!view->renderer) {
        printf("Failed to create SDL renderer: %s", SDL_GetError());
        free(view);
        return NULL;
    }

    // Populate the view data structure.
    view->window = window;
    view->port = port;
    view->mutex = SDL_CreateMutex();
    view->draw_function = draw_function;
    view->data = data;

    // Print framerate every second
    view->fps = 0;
    view->fps_counter_timer = SDL_AddTimer(1000, view_fps_counter_callback, view);

    // Start the view thread.
    view->thread = interval_thread_create(view_draw, view, 7, "View");

    return view;
}

void view_draw(void *data)
{
    View *view = data;
    SDL_LockMutex(view->mutex);

    // Clear the screen
    SDL_SetRenderDrawColor(view->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(view->renderer);

    // Update the view.
    view_port_update(view->port);
    view_draw_grid(view);

    // Call the drawing callback function.
    view->draw_function(view, view->data);

    // Draw!
    SDL_RenderPresent(view->renderer);

    // Increment the frame count.
    view->fps++;

    SDL_UnlockMutex(view->mutex);
}

uint32_t view_fps_counter_callback(uint32_t interval, void *data)
{
    View *view = data;

    // Get the number of frames rendered since the last call, and reset the 
    // frame counter.
    SDL_LockMutex(view->mutex);
    int fps = view->fps;
    view->fps = 0;
    SDL_UnlockMutex(view->mutex);

    printf("FPS: %i\n", fps);
    return interval;
}

void view_resize_window(View *view, int x, int y)
{
    if (!view) {
        return;
    }

    // Window width and height must be greater than 0.
    if (x <= 0 || y <= 0) {
        return;
    }

    /// @BUG: Resizing doesn't work unless the renderer is recreated.
    SDL_LockMutex(view->mutex);
    SDL_DestroyRenderer(view->renderer);
    view->renderer = SDL_CreateRenderer(
        view->window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    view->port->screen = (Vector){x / 2, y / 2};
    SDL_UnlockMutex(view->mutex);
}

void view_move(View *view, Direction direction, bool state)
{
    SDL_LockMutex(view->mutex);
    switch (direction)
    {
        case DIRECTION_NORTH: view_port_move_up(view->port,    state); break;
        case DIRECTION_EAST:  view_port_move_right(view->port, state); break;
        case DIRECTION_SOUTH: view_port_move_down(view->port,  state); break;
        case DIRECTION_WEST:  view_port_move_left(view->port,  state); break;
        case DIRECTION_IN:    view_port_move_in(view->port,    state); break;
        case DIRECTION_OUT:   view_port_move_out(view->port,   state); break;
        default: break;
    }
    SDL_UnlockMutex(view->mutex);
}

void view_set_position(View *view, Vector pos)
{
    SDL_LockMutex(view->mutex);
    view->port->position.x = pos.x;
    view->port->position.y = pos.y;
    SDL_UnlockMutex(view->mutex);
}

void view_draw_grid(View *view)
{
    /// @todo: Update grid to new size when it gets to dense or sparse.

    // Calculate world space bounds.
    double x1 = view->port->position.x - view->port->dimensions.x;
    double y1 = view->port->position.y - view->port->dimensions.y;
    double x2 = view->port->position.x + view->port->dimensions.x;
    double y2 = view->port->position.y + view->port->dimensions.y;

    SDL_SetRenderDrawColor(view->renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);

    for (int x = round(x1), end = round(x2) + 1; x < end; x++) {
        Vector p1 = view_world_to_port(view->port, (Vector){x, y1});
        Vector p2 = view_world_to_port(view->port, (Vector){x, y2});
        SDL_RenderDrawLine(view->renderer, p1.x,  p1.y, p2.x,  p2.y);
    }

    for (int y = round(y1), end = round(y2) + 1; y < end; y++) {
        Vector p1 = view_world_to_port(view->port, (Vector){x1, y});
        Vector p2 = view_world_to_port(view->port, (Vector){x2, y});
        SDL_RenderDrawLine(view->renderer, p1.x,  p1.y, p2.x,  p2.y);
    }
}

void view_destroy(View *view)
{
    if (!view)
        return;

    // Notify and wait for drawing thread to terminate.
    interval_thread_destroy(view->thread);

    // Stop the condition variable being triggered.
    SDL_RemoveTimer(view->fps_counter_timer);

    // Destroy the view.
    SDL_DestroyRenderer(view->renderer);
    SDL_DestroyMutex(view->mutex);

    free(view);
}
