#include "model/model.h"

#include <math.h>
#include <stdio.h>

#include "SDL2/SDL.h"

#include "util/array.h"
#include "util/random.h"
#include "util/vector.h"
#include "util/time.h"
#include "util/intervalthread.h"
#include "model/asteroid.h"

/**
 * Struct containing Model control related data.
 */
struct Model {
    Array *asteroids;
    Array *colliding;
    Time time_last;
    SDL_mutex *mutex;
    IntervalThread *thread;
    bool paused;
};

Model *model_create()
{
    // Seed random for this thread.
    random_seed();

    // Allocate a buffer for the model structure.
    Model *model = malloc(sizeof(Model));
    if (!model)
        return NULL;

    Array *asteroids = array_create(sizeof(Asteroid*));
    Array *colliding = array_create(sizeof(bool));

    for (int i = 0; i < 10; ++i) {

        // Create an asteroid at a random location.
        Asteroid *asteroid = asteroid_create();
        asteroid->object->position = (Vector){
            random_double(-5, 5),
            random_double(-5, 5)
        };

        asteroid->object->velocity = (Vector){
            random_double(-0.03, 0.03),
            random_double(-0.03, 0.03)
        };

        asteroid->object->omega = random_double(-0.03, 0.03);

        // Add it to the array of asteroids.
        array_push_back(asteroids, (void*)&asteroid);

        // Initially set to not colliding.
        bool t = false;
        array_push_back(colliding, &t);
    }

    model->asteroids = asteroids;
    model->colliding = colliding;
    model->time_last = time_global();
    model->paused = false;

    model->mutex = SDL_CreateMutex();

    model->thread = interval_thread_create(model_increment, model, 7);

    return model;
}

void model_increment(void *data)
{
    Model *model = data;

    // Lock access to the model data and advance the model.
    SDL_LockMutex(model->mutex);

    Time elapsed_ms = time_since_last(&model->time_last);

    int n = array_length(model->asteroids);

    for (int i = 0; i < n; i++) {

        Asteroid *asteroid = *(Asteroid**)array_get(model->asteroids, i);

        if (!model->paused)
            asteroid_advance(asteroid, (double)elapsed_ms / 1000);

        if (asteroid->object->position.x < -5)
            asteroid->object->velocity.x *= -1.0;

        if (asteroid->object->position.x > 5)
            asteroid->object->velocity.x *= -1.0;

        if (asteroid->object->position.y < -5)
            asteroid->object->velocity.y *= -1.0;

        if (asteroid->object->position.y > 5)
            asteroid->object->velocity.y *= -1.0;
    }

    // Determine collisions.
    for (int i = 0; i < n; i++)
        *(bool*)array_get(model->colliding, i) = false;

    for (int i = 0; i < n; i++) {

        if (*(bool*)array_get(model->colliding, i))
            continue;

        Asteroid *A = *(Asteroid**)array_get(model->asteroids, i);

        for (int j = 0; j < n; j++) {

            // Skip checking if the same polygon is colliding with itself.
            if (i == j)
                continue;

            Asteroid *B = *(Asteroid**)array_get(model->asteroids, j);

            bool collision = false;
            Vector mtv;
            polygon_colliding(A->verticies, B->verticies, &collision, &mtv);

            if (collision) {
                *(bool*)array_get(model->colliding, i) = true;
                *(bool*)array_get(model->colliding, j) = true;
            }
        }
    }

    SDL_UnlockMutex(model->mutex);
}

void model_pause_toggle(Model *model)
{
    SDL_LockMutex(model->mutex);
    model->paused = !model->paused;
    SDL_UnlockMutex(model->mutex);
}

void model_draw_polygon(
    View *view,
    Array *polygon
) {
    Vector *a = NULL;
    Vector *b = NULL;

    if (!array_at_pointer(polygon, 0, (void*)&a))
        return;

    for (int i = 0; i < array_length(polygon); i++) {

        // Get the second coordinate.
        if (!array_at_pointer(polygon, (i + 1) % array_length(polygon), (void*)&b))
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
    Asteroid **asteroids = array_data(model->asteroids);
    int n = array_length(model->asteroids);

    // Set to white lines.
    SDL_SetRenderDrawColor(view->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    // For each polygon.
    for (int i = 0; i < n; ++i) {

        Asteroid *asteroid = *(asteroids + i);
        Array *polygon = asteroid->verticies;

        bool colliding;
        array_at_copy(model->colliding, i, (void*)&colliding);

        if (colliding)
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

    Asteroid **asteroids = array_data(model->asteroids);
    int n = array_length(model->asteroids);

    // Destroy the polygons.
    for (int i = 0; i < n; i++) {
        asteroid_destroy(*(asteroids + i));
    }

    // Deallocate the arrays.
    array_destroy(model->asteroids);
    array_destroy(model->colliding);

    SDL_DestroyMutex(model->mutex);

    free(model);
}