#include "SDL2/SDL.h"

#include <stdio.h>

#include "controller.h"

int main(int argc, char* argv[])
{
    // Initialise SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Failed to initialise SDL: %s. Exiting.", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    // Create the controller.
    Controller *controller = controller_create();

    // Run the controller event handling thread.
    controller_thread(controller);

    // Destroy the controller.
    controller_destroy(controller);

    SDL_Quit();
    return 0;
}
