#include "view.h"

#include <stdio.h>

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
    SDL_TimerID framerate_timer;
    // Pointer to the model to draw.
    Spin *spin;
    // Mutex to stop the controller and view from accessing done concurrently.
    SDL_mutex *done_mutex;
    // Flag to indicate that the view thread should terminate.
    bool done;
};

uint32_t view_notify_callback(uint32_t interval, void *view)
{
    view_notify(view);
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
    view->done_mutex = SDL_CreateMutex();
    view->done = false;

    // Add a timer, that calls back to the view_notify_callback() function
    // every 17 milliseconds (approximately 144Hz).
    view->framerate_timer = SDL_AddTimer(7, view_notify_callback, view);

    // Success! Start the drawing thread and return the view.
    view->thread = SDL_CreateThread(view_thread, NULL, view);

    return view;
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
    SDL_CondSignal(view->draw_condition);
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
        spin_view_update(view->renderer, view->spin);

        // Draw!
        SDL_RenderPresent(view->renderer);

        // Check after renderering if the view thread should be exited.
        if (view_done(view)) {
            SDL_UnlockMutex(view->mutex);
            return 0;
        }

        // Wait until we should draw the
        SDL_CondWaitTimeout(view->draw_condition, view->mutex, 1 / 60 * 1000);
    }

    return 0;
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
    SDL_RemoveTimer(view->framerate_timer);

    // Destroy the view.
    SDL_DestroyRenderer(view->renderer);
    SDL_DestroyMutex(view->mutex);
    SDL_DestroyCond(view->draw_condition);
    SDL_DestroyMutex(view->done_mutex);
    free(view);
}