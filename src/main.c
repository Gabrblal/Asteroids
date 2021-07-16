#include "SDL2/SDL.h"

#include <stdio.h>

#include "controller.h"
#include "util/timer.h"

int main(int argc, char* argv[])
{
    // Initialise SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Failed to initialise SDL: %s. Exiting.", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    // Initialise the timer interface.
    time_initialise();

    // Create the controller, run the controller event handling thread and
    // then destroy the controller, that cascades to destroy the rest of the
    // application. The controller thread will exit when an exit event is
    // received.
    Controller *controller = controller_create();
    controller_thread(controller);
    controller_destroy(controller);

    // Deinitialise the timer interface.
    time_deinitialise();

    // Deinitialise SDL.
    SDL_Quit();
    return 0;
}
