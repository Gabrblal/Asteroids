#include "view.h"

#include <stdint.h>
#include <stdio.h>

#include "spin/spin_view.h"

/**
 * @private
 */
struct View {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_mutex *mutex;
    SDL_Thread *thread;
    SDL_cond *draw_condition;
    SDL_TimerID framerate_timer;
    Spin *spin;
    SDL_mutex *done_mutex;
    bool done;
};

/**
 * @private
 * 
 * Checks if the current view has exited, and should be destroyed.
 * 
 * @return If the view should be exited.
 */
bool view_done(View *view)
{
    SDL_LockMutex(view->done_mutex);
    bool done = view->done;
    SDL_UnlockMutex(view->done_mutex);

    return done;
}

void view_notify(View *view)
{
    SDL_CondSignal(view->draw_condition);
}

uint32_t view_notify_callback(uint32_t interval, void *view)
{
    view_notify(view);
    return interval;
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

View *view_create(SDL_Window *window, Spin *spin)
{
    if (!window || !spin)
        return NULL;

    View *view = malloc(sizeof(View));
    if (!view) {
        return NULL;
    }

    view->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!view->renderer) {
        printf("Failed to create SDL renderer: %s", SDL_GetError());
        free(view);
        return NULL;
    }

    view->window = window;
    view->mutex = SDL_CreateMutex();
    view->draw_condition = SDL_CreateCond();
    view->framerate_timer = SDL_AddTimer(17, view_notify_callback, view);
    view->spin = spin;
    view->done_mutex = SDL_CreateMutex();
    view->done = false;

    view->thread = SDL_CreateThread(view_thread, NULL, view);
    
    return view;
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