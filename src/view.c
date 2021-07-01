#include "view.h"

#include <stdio.h>
#include <math.h>

#include "spin/spin.h"

/**
 * @struct ViewPort
 * 
 * Struct containing data on where the current view port is. That is, what area
 * of the worldspace is currently being viewed.
 */ 
typedef struct {
    // The x coordinate of the view in world space (meters).
    float x;
    // The y coordinate of the view in world space (meters).
    float y;
    // The width of the view, from the x coordiate to the right (meters).
    float width;
    // The height of the view, from the y coordinat down (meters).
    float height;
    // Current pixel width of the screen.
    int screen_width;
    // Current pixel height of the screen.
    int screen_height;
} ViewPort;

/**
 * @struct View
 * 
 * Struct containing all data related to the view.
 */
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
    ViewPort port;
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
    view->port = (ViewPort){-2.0, -2.0, 4.0, 4.0, 600, 600};
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

        // Set render colour to white.
        SDL_SetRenderDrawColor(view->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

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

bool view_done(View *view)
{
    SDL_LockMutex(view->done_mutex);
    bool done = view->done;
    SDL_UnlockMutex(view->done_mutex);

    return done;
}

void view_notify(View *view)
{
    // Signal view thread to draw.
    SDL_CondBroadcast(view->draw_condition);
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
    SDL_UnlockMutex(view->mutex);
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

    p1 = view_world_to_port(view, p1);
    p2 = view_world_to_port(view, p2);
    p3 = view_world_to_port(view, p3);

    SDL_RenderDrawLine(view->renderer, p1.x,  p1.y, p2.x,  p2.y);
    SDL_RenderDrawLine(view->renderer, p2.x,  p2.y, p3.x,  p3.y);
    SDL_RenderDrawLine(view->renderer, p3.x,  p3.y, p1.x,  p1.y);
}

Vector2 view_port_to_world(View *view, Vector2 pixel)
{
    Vector2 world = {
        pixel.x / view->port.screen_width * view->port.width + view->port.x,
        pixel.y / view->port.screen_height * view->port.height + view->port.y
    };

    return world;
}

Vector2 view_world_to_port(View *view, Vector2 coordinate)
{
    Vector2 pixel = {
        (coordinate.x - view->port.x) * view->port.screen_width / view->port.width,
        (coordinate.y - view->port.y) * view->port.screen_height / view->port.height
    };

    return pixel;
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