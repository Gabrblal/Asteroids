#include "spin/spin.h"

bool spin_done(Spin *spin)
{
    SDL_LockMutex(spin->mutex);
    bool done = spin->done;
    SDL_UnlockMutex(spin->mutex);

    return done;
}

void spin_increment(Spin *spin)
{
    SDL_LockMutex(spin->mutex);
    spin->data.theta += 0.001;
    SDL_UnlockMutex(spin->mutex);
}

int spin_thread(void *data)
{
    Spin *spin = data;
    while (true)
    {
        if (spin_done(spin))
            return 0;

        spin_increment(spin);

        SDL_Delay(10);
    }
}

Spin *spin_create(int x, int y, double R, double theta)
{
    Spin *spin = malloc(sizeof(Spin));

    // Set variables.
    spin->mutex = SDL_CreateMutex();
    spin->data.x = x;
    spin->data.y = y;
    spin->data.R = R;
    spin->data.theta = theta;
    spin->done = false;

    // Create and start the model thread.
    spin->thread = SDL_CreateThread(spin_thread, NULL, spin);

    return spin;
}

SpinData spin_get(Spin *spin)
{
    // Retrieve data under lock.
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