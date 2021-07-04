#include "model/model.h"

#include <math.h>

#include "SDL2/SDL.h"
#include "util/list.h"
#include "util/vector2.h"

/**
 * The possible state of the modelning line.
 */
typedef struct {
    Vector2 u;
    Vector2 v;
    double theta;
    double omega;
} ModelData;

/**
 * Struct containing Model control related data.
 */
struct Model {
    // Mutex protecting concurrent access to the Model control data.
    SDL_mutex *mutex;
    // Thread simulating the model.
    SDL_Thread *thread;
    // Model data.
    ModelData data;
    // Flag indicating for the model thread to exit.
    bool done;
};

Model *model_create()
{
    // Allocate a buffer for the model structure.
    Model *model = malloc(sizeof(Model));

    model->data.u = (Vector2){1.5 * M_PI_4, 1.5 * M_PI_4};
    model->data.v = (Vector2){M_PI_4, M_PI_4};
    model->data.theta = M_PI_4;
    model->data.omega = 0.00000005;

    // Set model variables.
    model->mutex = SDL_CreateMutex();
    model->done = false;

    // Create and start the model thread.
    model->thread = SDL_CreateThread(model_thread, NULL, model);

    return model;
}

bool model_done(Model *model)
{
    // Lock access to the model data and access if the model thread has finished
    // for this Model object.
    SDL_LockMutex(model->mutex);
    bool done = model->done;
    SDL_UnlockMutex(model->mutex);

    return done;
}

void model_increment(Model *model)
{
    // Lock access to the model data and advance the model.
    SDL_LockMutex(model->mutex);
    model->data.theta += model->data.omega;
    if (model->data.theta > 2 * M_PI) {
        model->data.theta = 0;
    }
    SDL_UnlockMutex(model->mutex);
}

int model_thread(void *data)
{
    Model *model = data;
    while (true)
    {
        // If the thread should exit, then return.
        if (model_done(model))
            return 0;

        // Advance the model.
        model_increment(model);
    }
}

void model_apply(Model *model, void(*func)(void*, void*), void *data)
{
    SDL_LockMutex(model->mutex);
    func(model, data);
    SDL_UnlockMutex(model->mutex);
}

void model_draw(View *view, void *data)
{
    Model *model = data;

    // Get the centre point.
    Vector2 rotated = vector2_rot(model->data.u, model->data.theta);

    Vector2 c =  view_world_to_port(view->port, (Vector2){0, 0});
    Vector2 u = view_world_to_port(view->port, rotated);
    Vector2 v = view_world_to_port(view->port, model->data.v);
    Vector2 proj = view_world_to_port(
        view->port,
        vector2_proj(rotated, model->data.v)
    );

    SDL_SetRenderDrawColor(view->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(view->renderer, c.x,  c.y, u.x,  u.y);
    SDL_RenderDrawLine(view->renderer, c.x,  c.y, v.x,  v.y);

    SDL_SetRenderDrawColor(view->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(view->renderer, c.x,  c.y, proj.x,  proj.y);
}

void model_destroy(Model *model)
{
    // Indicate that the model model should be stopped.
    SDL_LockMutex(model->mutex);
    model->done = true;
    SDL_UnlockMutex(model->mutex);

    // Wait for the thread to exit
    SDL_WaitThread(model->thread, NULL);

    // Deallocate.
    SDL_DestroyMutex(model->mutex);
    free(model);
}