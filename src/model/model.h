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
 * Advance the model. Called continuously by spin_thread(). 
 * 
 * Thread safe.
 * 
 * @param model The model instance to advance by one step.
 */
void model_increment(void *model);

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