#include "view.h"

#include <stdio.h>
#include <math.h>

#include "view/view_port.h"

struct View {
    // Main game window, used for making the renderer.
    SDL_Window *window;
    // Renderer used to clear, draw and render the screen.
    SDL_Renderer *renderer;
    // View mutex, to prevent data access during drawing, or concurrent drawing.
    SDL_mutex *mutex;
    // Thread drawing to the window.
    SDL_Thread *thread;
    // Condition variable to force a draw from controller, and to wait on to 
    // prevent continuous draw calls that are resource intensive.
    SDL_cond *draw_condition;
    // SDL timer that calls a callback to notify draw_condition, to draw to the 
    // window at a desired rate (60Hz).
    SDL_TimerID framerate_control_timer;
    // Timer that calls a callback to print how many frames have been rednered
    // in the last second, and reset the fps counter.
    SDL_TimerID fps_counter_timer;
    // Frames per second, locked under View->mutex.
    int fps;
    // The current view area of the model.
    ViewPort *port;
    // Mutex to stop the controller and view from accessing done concurrently.
    SDL_mutex *done_mutex;
    // Flag to indicate that the view thread should terminate.
    bool done;
    // Pointer to the model to draw.
    Spin *spin;
};

// Callback to notify the view to draw.
uint32_t view_notify_callback(uint32_t interval, void *data)
{
    view_notify(data);
    return interval;
}
 
// Callback to print the frames per second.
uint32_t view_fps_counter_callback(uint32_t interval, void *data)
{
    View *view = data;

    SDL_LockMutex(view->mutex);
    int fps = view->fps;
    view->fps = 0;
    SDL_UnlockMutex(view->mutex);

    printf("FPS: %i\n", fps);
    return interval;
}

View *view_create(SDL_Window *window, Spin *spin)
{
    // Prevent null pointers.
    if (!window || !spin)
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
    view->mutex = SDL_CreateMutex();
    view->draw_condition = SDL_CreateCond();
    view->spin = spin;
    view->fps = 0;
    view->port = view_port_create(
        (Vector2){-2.0, -2.0},
        (Vector2){4.0, 4.0},
        (Vector2){300, 300},
        0.0001, 0.001
    );
    view->done_mutex = SDL_CreateMutex();
    view->done = false;

    // Add a timer, that calls back to the view_notify_callback() function
    // every 17 milliseconds (approximately 144Hz).
    view->framerate_control_timer = SDL_AddTimer(7, view_notify_callback, view);

    // Print framerate every second.
    view->fps_counter_timer = SDL_AddTimer(1000, view_fps_counter_callback, view);

    // Success! Start the drawing thread and return the view.
    view->thread = SDL_CreateThread(view_thread, NULL, view);

    return view;
}

int view_thread(void *data)
{
    View *view = data;

    while (!view_done(view)) {
        SDL_LockMutex(view->mutex);

        // Clear the screen
        SDL_SetRenderDrawColor(view->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(view->renderer);

        // Update the view.
        view_port_update(view->port);

        view_draw_grid(view);

        // Update the renderer.
        SpinData spin_data = spin_get(view->spin);
        spin_view_update(view, &spin_data);

        // Draw!
        SDL_RenderPresent(view->renderer);
        view->fps++;

        // Wait until we should draw the next frame.
        SDL_CondWait(view->draw_condition, view->mutex);

        // After this call the mutex is locked, and will be locked a second time
        // at the top of loop. SDL mutexes are recursive (must unlock for every
        // lock called on a mutex), so we must unlock it again.
        SDL_UnlockMutex(view->mutex);
    }

    return 0;
}

void view_notify(View *view)
{
    // Signal view thread to draw.
    SDL_CondBroadcast(view->draw_condition);
}

bool view_done(View *view)
{
    SDL_LockMutex(view->done_mutex);
    bool done = view->done;
    SDL_UnlockMutex(view->done_mutex);

    return done;
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
    view->port->screen = (Vector2){x / 2, y / 2};
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
        Vector2 p1 = view_world_to_port(view->port, (Vector2){x, y1});
        Vector2 p2 = view_world_to_port(view->port, (Vector2){x, y2});
        SDL_RenderDrawLine(view->renderer, p1.x,  p1.y, p2.x,  p2.y);
    }

    for (int y = round(y1), end = round(y2) + 1; y < end; y++) {
        Vector2 p1 = view_world_to_port(view->port, (Vector2){x1, y});
        Vector2 p2 = view_world_to_port(view->port, (Vector2){x2, y});
        SDL_RenderDrawLine(view->renderer, p1.x,  p1.y, p2.x,  p2.y);
    }
}

void spin_view_update(View *view, SpinData *spin)
{
    double s = sin(spin->theta);
    double c = cos(spin->theta);

    Vector2 p1 = {
        spin->p1.x * c - spin->p1.y * s,
        spin->p1.x * s + spin->p1.y * c
    };

    Vector2 p2 = {
        spin->p2.x * c - spin->p2.y * s,
        spin->p2.x * s + spin->p2.y * c
    };

    Vector2 p3 = {
        spin->p3.x * c - spin->p3.y * s,
        spin->p3.x * s + spin->p3.y * c
    };

    p1 = view_world_to_port(view->port, p1);
    p2 = view_world_to_port(view->port, p2);
    p3 = view_world_to_port(view->port, p3);

    SDL_SetRenderDrawColor(view->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(view->renderer, p1.x,  p1.y, p2.x,  p2.y);
    SDL_RenderDrawLine(view->renderer, p2.x,  p2.y, p3.x,  p3.y);
    SDL_RenderDrawLine(view->renderer, p3.x,  p3.y, p1.x,  p1.y);
}

void view_destroy(View *view)
{
    if (!view)
        return;

    // Notify and wait for drawing thread to terminate.
    SDL_LockMutex(view->done_mutex);
    view->done = true;
    SDL_UnlockMutex(view->done_mutex);
    view_notify(view);
    SDL_WaitThread(view->thread, NULL);

    // Stop the condition variable being triggered.
    SDL_RemoveTimer(view->framerate_control_timer);
    SDL_RemoveTimer(view->fps_counter_timer);

    // Destroy the view.
    SDL_DestroyRenderer(view->renderer);
    SDL_DestroyMutex(view->mutex);
    SDL_DestroyCond(view->draw_condition);
    SDL_DestroyMutex(view->done_mutex);
    free(view);
}