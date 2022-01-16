#include "util/random.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "SDL2/SDL.h"

SDL_mutex *s_rand_mutex = NULL;

void random_initialise()
{
    s_rand_mutex = SDL_CreateMutex();
    SDL_LockMutex(s_rand_mutex);
}

void random_seed()
{
    SDL_LockMutex(s_rand_mutex);
    srand(time(NULL));
    SDL_UnlockMutex(s_rand_mutex);
}

int random_int(int min, int max)
{
    SDL_LockMutex(s_rand_mutex);
    int random = min + rand() % abs(max - min + 1);
    SDL_UnlockMutex(s_rand_mutex);
    return random;
}

double random_double(double min, double max)
{
    SDL_LockMutex(s_rand_mutex);
    double random = min + (double)rand() / (double)RAND_MAX * (max - min);
    SDL_UnlockMutex(s_rand_mutex);
    return random;
}

void random_deinitialise()
{
    SDL_DestroyMutex(s_rand_mutex);
}
