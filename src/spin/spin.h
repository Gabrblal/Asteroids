#ifndef SPIN_H
#define SPIN_H

#include <stdbool.h>

#include "SDL2/SDL.h"

typedef struct {
    int x;
    int y;
    double R;
    double theta;
} SpinData;

typedef struct {
    SDL_mutex *mutex;
    SDL_Thread *thread;
    SpinData data;
    bool done;
} Spin;

/**
 * Create a new spin instance.
 */
Spin *spin_create(int x, int y, double R, double theta);

/**
 * Retrieve data from a Spin model. Sets all pointers to NULL.
 * 
 * @param spin The Spin model to access data from.
 * @return The spins current state.
 */
SpinData spin_get(Spin *spin);

/**
 * Deallocates memory allocated for spin. Using a spin object after destorying
 * it is undefined.
 * 
 * @param spin The Spin object to be destroyed.
 */
void spin_destroy(Spin *spin);

#endif