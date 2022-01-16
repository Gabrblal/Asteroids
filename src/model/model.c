#include "model/model.h"

#include <math.h>
#include <stdio.h>

#include "SDL2/SDL.h"

#include "util/array.h"
#include "util/random.h"
#include "util/vector.h"
#include "util/intervalthread.h"
#include "model/polygon.h"

/**
 * Struct containing Model control related data.
 */
struct Model {
    Array *polygons;
    Array *colliding;
    SDL_mutex *mutex;
    IntervalThread *thread;
};

Model *model_create()
{
    // Seed random for this thread.
    random_seed();

    // Allocate a buffer for the model structure.
    Model *model = malloc(sizeof(Model));
    if (!model)
        return NULL;

    Array *polygons = array_create(sizeof(Array));
    Array *colliding = array_create(sizeof(bool));

    for (int i = 0; i < 5; ++i) {

        // Create a random polygon.
        Array *polygon = polygon_create_random_regular(1.0);

        Vector position = {random_double(-5, 5), random_double(-5, 5)};
        for (int i = 0; i < array_length(polygon); i++) {

            // Get the vertex.
            Vector *p = NULL;
            array_at(polygon, i, (void*)&p);

            // Offset the vertex by the position.
            *p = vector_add(*p, position);
        }

        array_push_back(polygons, polygon);

        bool t = false;
        array_push_back(colliding, &t);
    }

    model->polygons = polygons;
    model->colliding = colliding;
    model->mutex = SDL_CreateMutex();
    model->thread = interval_thread_create(model_increment, model, 7);

    return model;
}

void model_increment(void *data)
{
    Model *model = data;

    // Lock access to the model data and advance the model.
    SDL_LockMutex(model->mutex);

    Array *polygons = array_data(model->polygons);
    bool *colliding = array_data(model->colliding);
    int n = array_length(model->polygons);

    for (int i = 0; i < n; i++) {
        Array *A = polygons + i;

        for (int j = 0; j < n; j++) {

            // Skip checking if the same polygon is colliding with itself.
            if (i == j)
                continue;

            Array *B = polygons + j;

            bool is_colliding = false;
            polygon_colliding(A, B, &is_colliding);

            *(colliding + i) |= is_colliding;
        }
    }

    SDL_UnlockMutex(model->mutex);
}

void model_apply(Model *model, void(*func)(void*, void*), void *data)
{
    SDL_LockMutex(model->mutex);
    func(model, data);
    SDL_UnlockMutex(model->mutex);
}

void model_draw_polygon(
    View *view,
    Array *polygon
) {
    Vector *a = NULL;
    Vector *b = NULL;

    if (!array_at(polygon, 0, (void*)&a))
        return;

    for (int i = 0; i < array_length(polygon); i++) {

        // Get the second coordinate.
        if (!array_at(polygon, (i + 1) % array_length(polygon), (void*)&b))
            return;

        Vector a_pixel = view_world_to_port(view->port, *a);
        Vector b_pixel = view_world_to_port(view->port, *b);

        SDL_RenderDrawLine(
            view->renderer,
            a_pixel.x, 
            a_pixel.y,
            b_pixel.x, 
            b_pixel.y
        );

        a = b;
    }
}

void model_draw(View *view, void *data)
{
    Model *model = data;
    SDL_LockMutex(model->mutex);

    // Dereference required data once.
    Array *polygons = model->polygons;

    // Set to white lines.
    SDL_SetRenderDrawColor(view->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    // For each polygon.
    for (int i = 0; i < array_length(polygons); ++i) {
        // Get the polygon.
        Array *polygon = NULL;
        array_at(polygons, i, (void*)&polygon);

        bool *colliding = NULL;
        array_at(model->colliding, i, (void*)&colliding);

        if (*colliding)
            SDL_SetRenderDrawColor(view->renderer, 255, 0, 0, 255);
        else
            SDL_SetRenderDrawColor(view->renderer, 255, 255, 255, 255);

        // Draw it.
        model_draw_polygon(view, polygon);
    }

    SDL_UnlockMutex(model->mutex);
}

void model_destroy(Model *model)
{
    interval_thread_destroy(model->thread);

    // Destroy the polygons.
    for (int i = 0; i < array_length(model->polygons); i++) {
        Array *polygon = NULL;

        if (!array_at(model->polygons, 0, (void*)&polygon))
            break;

        free(polygon);
    }

    // Deallocate the arrays.
    array_destroy(model->polygons);
    array_destroy(model->colliding);

    free(model);
}