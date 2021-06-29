#include "controller.h"

#include "SDL2/SDL.h"
#include "view.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL2/SDL.h"

/**
 * Struct containing all data related to the controller.
 */
struct Controller {
    SDL_Window *window;    // Window the program is running in.
    View *view;            // View of the model.
    Spin *model;           // Model.
    bool done;             // If the controller is done.
};

void controller_handle_key_up(SDL_Event *event)
{

}

void controller_handle_key_down(SDL_Event *event)
{

}

void controller_handle_mouse_up(SDL_Event *event)
{

}

void controller_handle_mouse_down(SDL_Event *event)
{

}

void controller_handle_mouse_wheel(SDL_Event *event)
{

}

void controller_handle_mouse_motion(SDL_Event *event)
{

}

void controller_handle_event(Controller *controller, SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_KEYUP           : controller_handle_key_up(event); break;
        case SDL_KEYDOWN         : controller_handle_key_down(event); break;
        case SDL_MOUSEBUTTONUP   : controller_handle_mouse_up(event); break;
        case SDL_MOUSEBUTTONDOWN : controller_handle_mouse_down(event); break;
        case SDL_MOUSEWHEEL      : controller_handle_mouse_wheel(event); break;
        case SDL_MOUSEMOTION     : controller_handle_mouse_motion(event); break;
        case SDL_QUIT : {
            controller->done = true;
            break;
        }
        default: break;
    }
}

void controller_thread(Controller *controller)
{
    SDL_Event event;
    while (true) {

        int timed_out = SDL_WaitEventTimeout(&event, 20);
        if (!timed_out) {
            controller_handle_event(controller, &event);
        }

        if (controller->done)
            return;
    }
}

Controller *controller_create()
{
    /// TODO: Loop through modes and select the window size, minus 100 pixels
    /// in all directions.

    // Create the window.
    SDL_Window *window = NULL;
    window = SDL_CreateWindow(
        "Asteroids",
        600, 600,
        600, 600,
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

void controller_destroy(Controller *controller)
{
    // Disregard null pointers.
    if (!controller)
        return;

    // Destroy everything in the controller.
    view_destroy(controller->view);
    spin_destroy(controller->model);
    SDL_DestroyWindow(controller->window);

    free(controller);
}
