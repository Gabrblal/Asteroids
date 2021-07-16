#include "util/timer.h"

#include <stdlib.h>

#include "SDL2/SDL.h"

// Time incrementing the current global time.
SDL_TimerID s_time_timer = -1;

// Mutex protecting concurrent access to the global time.
SDL_mutex *s_time_mutex = NULL;

// The current global time.
Time s_time = 0;

uint32_t time_s_callback(uint32_t interval, void *data)
{
    SDL_LockMutex(s_time_mutex);
    ++s_time;
    SDL_UnlockMutex(s_time_mutex);
    return interval;
}

void time_initialise()
{
    s_time_mutex = SDL_CreateMutex();
    SDL_LockMutex(s_time_mutex);
    s_time_timer = SDL_AddTimer(1, time_s_callback, NULL);
    SDL_UnlockMutex(s_time_mutex);
}

Time time_global()
{
    SDL_LockMutex(s_time_mutex);
    Time now = s_time;
    SDL_UnlockMutex(s_time_mutex);
    return now;
}

Time time_since_last(Time *time)
{
    SDL_LockMutex(s_time_mutex);

    // Calculate the time elapsed since the last call.
    Time elapsed = s_time - *time;

    // Set the time when called last to now.
    *time = s_time;

    SDL_UnlockMutex(s_time_mutex);
    return elapsed;
}

void time_destroy(Time *time)
{
    free(time);
}

void time_deinitialise()
{
    SDL_DestroyMutex(s_time_mutex);
    SDL_RemoveTimer(s_time_timer);
}