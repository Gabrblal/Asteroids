#include "util/intervalthread.h"

#include <stdlib.h>
#include <string.h>

#include "SDL2/SDL.h"

struct IntervalThread {
    // The true thread to run.
    SDL_Thread *thread;
    // Mutex used for the condition wait.
    SDL_mutex *mutex;
    // Condition to wait on for the next function execution.
    SDL_cond *condition;
    // Pointer to the data to pass the intermittently executing function.
    void *data;
    // Function to call intermittently.
    void(*func)(void *data);
    // Timer to notify the condition variable to call the thread function 
    // every interval.
    SDL_TimerID timer;
    // Boolean to quit the thread with on destruction.
    bool done;
    // Optional name for the thread.
    char *name;
};

uint32_t interval_thread_timer_callback(uint32_t interval, void *data)
{
    IntervalThread *thread = data;

    // Signal the interval thread to stop waiting and run the executing
    // function.
    SDL_CondBroadcast(thread->condition);

    return interval;
}

int interval_thread_wrapper(void *data)
{
    IntervalThread *thread = data;

    while (true) {

        // Check whether the thread is finished. Prevent concurrent access with
        // interval_thread_destroy().
        SDL_LockMutex(thread->mutex);
        bool done = thread->done;
        SDL_UnlockMutex(thread->mutex);

        // If the thread is finished, return.
        if (done)
            return 0;

        // Call the executing function and forward the data.
        thread->func(thread->data);

        // Wait for the condition variable to be notified before running
        // the interval thread function again.
        SDL_LockMutex(thread->mutex);
        SDL_CondWait(thread->condition, thread->mutex);
        SDL_UnlockMutex(thread->mutex);
    }
}

IntervalThread *interval_thread_create(
    void(*func)(void *data),
    void *data,
    uint32_t interval,
    const char *name
) {
    IntervalThread *thread = malloc(sizeof(IntervalThread));
    if (!thread)
        return NULL;

    thread->mutex = SDL_CreateMutex();
    thread->condition = SDL_CreateCond();
    thread->data = data;
    thread->func = func;
    thread->timer = SDL_AddTimer(interval, interval_thread_timer_callback, thread);
    thread->done = false;

    if (name)
        thread->name = strdup(name);
    else
        thread->name = NULL;

    // Ensure the thread is the last attribute to be instantiated, to ensure the
    // thread function does not access data during construction.
    thread->thread = SDL_CreateThread(interval_thread_wrapper, NULL, thread);

    return thread;
}

void interval_thread_destroy(IntervalThread *thread)
{
    if (!thread)
        return;

    // Indicate that the model model should be stopped.
    SDL_LockMutex(thread->mutex);
    thread->done = true;
    SDL_UnlockMutex(thread->mutex);

    // Wait for the thread to exit
    SDL_WaitThread(thread->thread, NULL);

    // Destroy attributes.
    SDL_DestroyMutex(thread->mutex);
    SDL_DestroyCond(thread->condition);
    SDL_RemoveTimer(thread->timer);

    if (thread->name)
        free(thread->name);

    // Free memory.
    free(thread);
}
