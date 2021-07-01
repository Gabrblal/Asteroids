#include "spin/spin.h"

#include "SDL2/SDL.h"

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
    SpinData data;
    // Flag indicating for the model thread to exit.
    bool done;
};

Spin *spin_create(Vector2 p1, Vector2 p2, Vector2 p3)
{
    // Allocate a buffer for the spin structure.
    Spin *spin = malloc(sizeof(Spin));

    // Set spin variables.
    spin->mutex = SDL_CreateMutex();
    spin->data.p1 = p1;
    spin->data.p2 = p2;
    spin->data.p3 = p3;
    spin->data.theta = 0;
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

void spin_increment(Spin *spin)
{
    // Lock access to the spin data and advance the model.
    SDL_LockMutex(spin->mutex);
    spin->data.theta += 0.0000001;
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

SpinData spin_get(Spin *spin)
{
    // Lock access to the spin data and copy it out.
    SDL_LockMutex(spin->mutex);
    SpinData result = spin->data;
    SDL_UnlockMutex(spin->mutex);

    return result;
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