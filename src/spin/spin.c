#include "spin/spin.h"

#include <math.h>

#include "SDL2/SDL.h"
#include "util/list.h"

/**
 * Struct containing Spin related data. Spin is simply a model of a rotating
 * line.
 */
struct Spin {
    // Mutex protecting concurrent access to the Spin data, especially theta.
    SDL_mutex *mutex;
    // Thread updating the spin model.
    SDL_Thread *thread;
    // The current state of the line, it's angle pointing.
    List *triangles;
    // Flag indicating for the model thread to exit.
    bool done;
};

Spin *spin_create()
{
    // Allocate a buffer for the spin structure.
    Spin *spin = malloc(sizeof(Spin));
    spin->triangles = list_create(sizeof(SpinData));

    SpinData data[] = {
        {{1.0, 0.0},   {-1.0, 1.0},  {-1.0, -1.0}, {-2.0, -2.0},   0.0, 0.0,  0.0000002, 0.00000005},
        {{-1.0, 0.0},  {-1.0, -1.0}, {0.0, 0.0},   {-4.0, 2.0},    0.0, 1.0,  0.0000003, 0.000000005},
        {{0.0, 0.6},   {-0.5, -0.5},  {0.5, 0.0},   {2.0, -3.0},    0.0, 2.0, 0.000003, 0.00000002},
        {{-2.0, -3.0}, {0.0, -2.0},  {0.0, 0.0},   {5.0, 2.0},     0.0, 3.0,  0.00000025, 0.00000004},
        {{-2.0, 2.0},  {0.0, 0.0},   {-1.0, -1.0}, {0.0, 4.0},     0.0, 4.0,  0.00000035, 0.0000002},
        {{2.0, 0.0},   {-2.0, 0.0},  {0.0, 2.0},   {0.0, -2.0},    0.0, 5.0,  0.00000055, 0.00000008},
        {{-1.0, 1.0},  {1.0, 0.0},   {-1.0, 0.0},  {2.0, 0.0},     0.0, 6.0,  0.00000045, 0.00000003}
    };

    for (int i = 0; i < 7; ++i) {
        list_push_back(spin->triangles, data + i);
    }

    // Set spin variables.
    spin->mutex = SDL_CreateMutex();
    spin->done = false;

    // Create and start the model thread.
    spin->thread = SDL_CreateThread(spin_thread, NULL, spin);

    return spin;
}

bool spin_done(Spin *spin)
{
    // Lock access to the spin data and access if the spin thread has finished
    // for this Spin object.
    SDL_LockMutex(spin->mutex);
    bool done = spin->done;
    SDL_UnlockMutex(spin->mutex);

    return done;
}

void spin_increment_once(void *element, void *data)
{
    SpinData *spin = element;
    spin->theta1 = spin->theta1 > 6.2831853 ? 0 : spin->theta1 + spin->omega1;
    spin->theta2 = spin->theta2 > 6.2831853 ? 0 : spin->theta2 + spin->omega2;
}

void spin_increment(Spin *spin)
{
    // Lock access to the spin data and advance the model.
    SDL_LockMutex(spin->mutex);
    list_apply(spin->triangles, spin_increment_once, NULL);
    SDL_UnlockMutex(spin->mutex);
}

int spin_thread(void *data)
{
    Spin *spin = data;
    while (true)
    {
        // If the thread should exit, then return.
        if (spin_done(spin))
            return 0;

        // Advance the model.
        spin_increment(spin);
    }
}

void spin_apply(Spin *spin, void(*func)(void*, void*), void *data)
{
    // Lock access to the spin data and copy it out.
    SDL_LockMutex(spin->mutex);
    list_apply(spin->triangles, func, data);
    SDL_UnlockMutex(spin->mutex);
}

void spin_view_update_triangle(void *element, void *data)
{
    SpinData *spin = element;
    View *view = data;

    double s1 = sin(spin->theta1);
    double c1 = cos(spin->theta1);
    double s2 = sin(spin->theta2);
    double c2 = cos(spin->theta2);

    double pos_x = spin->pos.x * c2 - spin->pos.y * s2;
    double pos_y = spin->pos.x * s2 + spin->pos.y * c2;

    Vector2 p1 = {
        pos_x + spin->p1.x * c1 - spin->p1.y * s1,
        pos_y + spin->p1.x * s1 + spin->p1.y * c1
    };

    Vector2 p2 = {
        pos_x + spin->p2.x * c1 - spin->p2.y * s1,
        pos_y + spin->p2.x * s1 + spin->p2.y * c1
    };

    Vector2 p3 = {
        pos_x + spin->p3.x * c1 - spin->p3.y * s1,
        pos_y + spin->p3.x * s1 + spin->p3.y * c1
    };

    p1 = view_world_to_port(view->port, p1);
    p2 = view_world_to_port(view->port, p2);
    p3 = view_world_to_port(view->port, p3);

    SDL_SetRenderDrawColor(view->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(view->renderer, p1.x,  p1.y, p2.x,  p2.y);
    SDL_RenderDrawLine(view->renderer, p2.x,  p2.y, p3.x,  p3.y);
    SDL_RenderDrawLine(view->renderer, p3.x,  p3.y, p1.x,  p1.y);
}

void spin_draw(View *view, void *data)
{
    Spin *spin = data;

    SpinData *first = NULL;
    list_at(spin->triangles, 0, (void*)&first);
    double s1 = sin(first->theta1);
    double c1 = cos(first->theta1);
    double s2 = sin(first->theta2);
    double c2 = cos(first->theta2);
    double pos_x = first->pos.x * c2 - first->pos.y * s2;
    double pos_y = first->pos.x * s2 + first->pos.y * c2;
    Vector2 p1 = {
        pos_x + first->p1.x * c1 - first->p1.y * s1,
        pos_y + first->p1.x * s1 + first->p1.y * c1
    };
    view_set_position(view, p1);

    spin_apply(spin, spin_view_update_triangle, view);
}

void spin_destroy(Spin *spin)
{
    // Indicate that the spin model should be stopped.
    SDL_LockMutex(spin->mutex);
    spin->done = true;
    SDL_UnlockMutex(spin->mutex);

    // Wait for the thread to exit
    SDL_WaitThread(spin->thread, NULL);

    // Deallocate.
    SDL_DestroyMutex(spin->mutex);
    free(spin);
}