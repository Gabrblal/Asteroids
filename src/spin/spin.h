#ifndef SPIN_H
#define SPIN_H

#include <stdbool.h>

#include "util/vector2.h"
#include "view/view.h"

/**
 * The possible state of the spinning line.
 */
typedef struct {
    Vector2 p1; // Line begin x coordinate.
    Vector2 p2; // Line begin y coordinate.
    Vector2 p3; // Line begin x coordinate.
    Vector2 pos; // Where to place the triangle.
    double theta1; // Current angle of the line.
    double theta2; // Current angle of the triangle.
    double omega1;
    double omega2;
} SpinData;

typedef struct Spin Spin;

/**
 * Create a new spin instance.
 */
Spin *spin_create();

/**
 * Apply a function to all triangles
 * 
 * @param func The function to apply.
 * @param data Optional data to pass to the function.
 */
void spin_apply(Spin *spin, void(*func)(void *, void *), void *data);

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
 * Draw a spin object to a renderer.
 * 
 * @param renderer The renderer to draw the spin model with.
 * @param spin The spin object to draw.
 */
void spin_draw(View *view, void *spin);

/**
 * Deallocates memory allocated for spin. Using a spin object after destorying
 * it is undefined.
 * 
 * @param spin The Spin object to be destroyed.
 */
void spin_destroy(Spin *spin);

#endif