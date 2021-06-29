#include "spin/spin_view.h"

#include <math.h>

void spin_view_update(SDL_Renderer *renderer, Spin *spin)
{
    SDL_LockMutex(spin->mutex);

    int x = spin->data.x + spin->data.R * cos(spin->data.theta);
    int y = spin->data.y + spin->data.R * sin(spin->data.theta);

    SDL_RenderDrawLine(renderer, spin->data.x,  spin->data.y, x, y);

    SDL_UnlockMutex(spin->mutex);
}
