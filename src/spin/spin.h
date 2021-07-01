#ifndef SPIN_H
#define SPIN_H

#include <stdbool.h>

#include "util/vector2.h"

/**
 * The possible state of the spinning line.
 */
typedef struct {
    Vector2 p1; // Line begin x coordinate.
    Vector2 p2; // Line begin y coordinate.
    Vector2 p3; // Line begin x coordinate.
    double theta; // Current angle of the line.
} SpinData;

typedef struct Spin Spin;

/**
 * Create a new spin instance.
 */
Spin *spin_create(Vector2 p1, Vector2 p2, Vector2 p3);

/**
 * Checks if the spin thread should exit. 
 * 
 * Thread safe.
 * 
 * @param spin The spin instance to check if it has exited.
 */
bool spin_done(Spin *spin);

/**
 * Advance the model of the spin. Called continuously by spin_thread(). 
 * 
 * Thread safe.
 * 
 * @param spin The spin instance to advance by one step.
 */
void spin_increment(Spin *spin);

/**
 * The thread function the spin model runs to simulate the spinning line. 
 * 
 * @param data Pointer to a buffer to pass to the thread function (Spin*).
 */
int spin_thread(void *data);

/**
 * Retrieve the current state of a Spin model.
 * 
 * @param spin The Spin model to access data from.
 * @return The spin's current state.
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