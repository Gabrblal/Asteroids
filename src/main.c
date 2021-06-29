#include "SDL2/SDL.h"

#include "controller.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    // Initialise SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Failed to initialise SDL: %s. Exiting.", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    Controller *controller = controller_create();
    controller_thread(controller);
    controller_destroy(controller);

    return 0;
}