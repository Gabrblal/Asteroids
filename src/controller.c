#include "controller.h"

#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "view.h"
#include "spin/spin.h"

/**
 * @struct Controller
 * 
 * Struct containing all data related to the controller.
 */
struct Controller {
    // Main game window, used for retrieving user events.
    SDL_Window *window;
    // Pointer to the view of the model.
    View *view;
    // Pointer to the model.
    Spin *model;
    // If the controller should exit or not.
    bool done;
};

Controller *controller_create()
{
    /// TODO: Loop through modes and select the window size, minus 100 pixels
    /// in all directions.

    // Create the window.
    SDL_Window *window = NULL;
    window = SDL_CreateWindow(
        "Asteroids",
        600, 600, // Position of window (x, y)
        600, 600, // Dimensions of window (width, height)
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        printf("Failed to create SDL window: %s. Exiting.", SDL_GetError());
        return NULL;
    }

    // Create the model.
    Spin *model = spin_create(300, 300, 100, 0);
    if (!model) {
        printf("Failed to create spin model. Exiting.");
        SDL_DestroyWindow(window);
        return NULL;
    }

    // Create the view of the model.
    View *view = view_create(window, model);
    if (!view) {
        printf("Failed to create view. Exiting.");
        spin_destroy(model);
        SDL_DestroyWindow(window);
        return NULL;
    }

    // Create the controller.
    Controller *controller = malloc(sizeof(Controller));
    if (!controller) {
        printf("Failed to create controller. Exiting.");
        view_destroy(view);
        spin_destroy(model);
        SDL_DestroyWindow(window);
        return NULL;
    }

    // Set the controller variables and return the controller.
    controller->window = window;
    controller->view = view;
    controller->model = model;
    controller->done = false;

    return controller;
}

void controller_thread(Controller *controller)
{
    SDL_Event event;
    while (true) {

        // Wait for the next event, and timeout after 20ms to check if the
        // controller should exit.
        int timed_out = SDL_WaitEventTimeout(&event, 20);

        // If the controller did not time out (it instead received an event)
        // then handle the event.
        if (!timed_out) {
            controller_handle_event(controller, &event);
        }

        // If an event caused the controller to exit, then exit.
        if (controller->done) {
            return;
        }
    }
}

void controller_handle_event(Controller *controller, SDL_Event *event)
{
    // Jump to the function that handles the specific type of event.
    switch (event->type)
    {
        case SDL_KEYUP           : controller_handle_key_up(controller, event); break;
        case SDL_KEYDOWN         : controller_handle_key_down(controller, event); break;
        case SDL_MOUSEBUTTONUP   : controller_handle_mouse_up(controller, event); break;
        case SDL_MOUSEBUTTONDOWN : controller_handle_mouse_down(controller, event); break;
        case SDL_MOUSEWHEEL      : controller_handle_mouse_wheel(controller, event); break;
        case SDL_MOUSEMOTION     : controller_handle_mouse_motion(controller, event); break;
        case SDL_QUIT : {
            // No mutex required since only this thread has access to this
            // variable.
            controller->done = true;
            break;
        }
        default: break;
    }
}

void controller_handle_key_up(Controller *controller, SDL_Event *event)
{}

void controller_handle_key_down(Controller *controller, SDL_Event *event)
{}

void controller_handle_mouse_up(Controller *controller, SDL_Event *event)
{}

void controller_handle_mouse_down(Controller *controller, SDL_Event *event)
{}

void controller_handle_mouse_wheel(Controller *controller, SDL_Event *event)
{}

void controller_handle_mouse_motion(Controller *controller, SDL_Event *event)
{}

void controller_destroy(Controller *controller)
{
    // Disregard null pointers.
    if (!controller)
        return;

    // Destroy everything in the controller.
    view_destroy(controller->view);
    spin_destroy(controller->model);
    SDL_DestroyWindow(controller->window);

    // Free controller memory.
    free(controller);
}
