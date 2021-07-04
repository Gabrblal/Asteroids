#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

#include "view/view.h"

typedef struct Model Model;

/**
 * Create a new model instance.
 */
Model *model_create();

/**
 * Apply a function to all triangles
 * 
 * @param func The function to apply.
 * @param data Optional data to pass to the function.
 */
void model_apply(Model *model, void(*func)(void *, void *), void *data);

/**
 * Checks if the model thread should exit. 
 * 
 * Thread safe.
 * 
 * @param model The model instance to check if it has exited.
 */
bool model_done(Model *model);

/**
 * Advance the model. Called continuously by spin_thread(). 
 * 
 * Thread safe.
 * 
 * @param model The model instance to advance by one step.
 */
void model_increment(Model *model);

/**
 * The thread function the model is simulated in. 
 * 
 * @param data Pointer to a buffer to pass to the thread function (Model*).
 */
int model_thread(void *data);

/**
 * Draw the model object to a renderer.
 * 
 * @param renderer Pointer to the renderer to draw the model with.
 * @param model Pointer to the model instance to draw.
 */
void model_draw(View *view, void *model);

/**
 * Deallocates memory allocated for a model. Using the model instance after
 * destorying it is undefined.
 * 
 * @param model The Model object to be destroyed.
 */
void model_destroy(Model *model);

#endif