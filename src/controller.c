#include "controller.h"

#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"
#include "view/view.h"
#include "model/model.h"
#include "util/vector2.h"

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
    Model *model;
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
        WINDOW_X, WINDOW_Y,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        printf("Failed to create SDL window: %s. Exiting.", SDL_GetError());
        return NULL;
    }

    // Create the model.
    Model *model = model_create();
    if (!model) {
        printf("Failed to create spin model. Exiting.");
        SDL_DestroyWindow(window);
        return NULL;
    }

    ViewPort *port = view_port_create(
        (Vector2){0.0, 0.0},
        (Vector2){2.0, 2.0},
        (Vector2){WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2},
        0.0008,
        0.02
    );

    // Create the view of the model.
    View *view = view_create(window, port, model_draw, model);
    if (!view) {
        printf("Failed to create view. Exiting.");
        model_destroy(model);
        SDL_DestroyWindow(window);
        return NULL;
    }

    // Create the controller.
    Controller *controller = malloc(sizeof(Controller));
    if (!controller) {
        printf("Failed to create controller. Exiting.");
        view_destroy(view);
        model_destroy(model);
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
        int received = SDL_WaitEventTimeout(&event, 20);

        // If the controller did not time out (it instead received an event)
        // then handle the event.
        if (received) {
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
        case SDL_WINDOWEVENT     : controller_handle_resize(controller, event); break;
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
{
    printf(
        "Up %s (0x%02X)\n",
        SDL_GetKeyName( event->key.keysym.sym ),
        event->key.keysym.scancode
    );

    switch (event->key.keysym.sym)
    {
        case SDLK_UP:    view_move(controller->view, DIRECTION_NORTH, false); break;
        case SDLK_RIGHT: view_move(controller->view, DIRECTION_EAST,  false); break;
        case SDLK_DOWN:  view_move(controller->view, DIRECTION_SOUTH, false); break;
        case SDLK_LEFT:  view_move(controller->view, DIRECTION_WEST,  false); break;
        case SDLK_e:     view_move(controller->view, DIRECTION_IN,    false); break;
        case SDLK_q:     view_move(controller->view, DIRECTION_OUT,   false); break;
        default: break;
    }
}

void controller_handle_key_down(Controller *controller, SDL_Event *event)
{
    printf(
        "Down %s (0x%02X)\n",
        SDL_GetKeyName( event->key.keysym.sym ),
        event->key.keysym.scancode
    );

    switch (event->key.keysym.sym)
    {
        case SDLK_UP:    view_move(controller->view, DIRECTION_NORTH, true); break;
        case SDLK_RIGHT: view_move(controller->view, DIRECTION_EAST,  true); break;
        case SDLK_DOWN:  view_move(controller->view, DIRECTION_SOUTH, true); break;
        case SDLK_LEFT:  view_move(controller->view, DIRECTION_WEST,  true); break;
        case SDLK_e:     view_move(controller->view, DIRECTION_IN,    true); break;
        case SDLK_q:     view_move(controller->view, DIRECTION_OUT,   true); break;
        case SDLK_ESCAPE: controller->done = true;
        default: break;
    }
}

void controller_handle_mouse_up(Controller *controller, SDL_Event *event)
{}

void controller_handle_mouse_down(Controller *controller, SDL_Event *event)
{}

void controller_handle_mouse_wheel(Controller *controller, SDL_Event *event)
{}

void controller_handle_mouse_motion(Controller *controller, SDL_Event *event)
{}

void controller_handle_resize(Controller *controller, SDL_Event *event)
{
    switch (event->window.event)
    {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
        case SDL_WINDOWEVENT_RESIZED: {
            view_resize_window(
                controller->view,
                event->window.data1,
                event->window.data2
            );
            break;
        }
        default: break;
    }
}

void controller_destroy(Controller *controller)
{
    // Disregard null pointers.
    if (!controller)
        return;

    // Destroy everything in the controller.
    view_destroy(controller->view);
    model_destroy(controller->model);
    SDL_DestroyWindow(controller->window);

    // Free controller memory.
    free(controller);
}
