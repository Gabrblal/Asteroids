#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "SDL2/SDL.h"

typedef struct Controller Controller;

/**
 * Creates a new controller instance. Instantiates the model and it's view. 
 * 
 * Must be called in main thread. After creating the controller, the 
 * controller_thread() function should be called in main to begin event 
 * handling.
 * 
 * @return Pointer to the main Controller instance, that contains the program
 * data.
 */
Controller *controller_create();

/**
 * The thread function the controller runs in to handle events. 
 * 
 * This function handles user input, and instructs the model to perform actions 
 * and the view to move around depending on what user input. 
 * 
 * @param controller Pointer to the main Controller instance.
 */
void controller_thread(Controller *controller);

/**
 * Handle a user event from a controller.
 * 
 * @param controller Pointer to controller instance in which the event occured.
 * @param event Pointer to the event data.
 */
void controller_handle_event(Controller *controller, SDL_Event *event);
void controller_handle_key_up(Controller *controller, SDL_Event *event);
void controller_handle_key_down(Controller *controller, SDL_Event *event);
void controller_handle_mouse_up(Controller *controller, SDL_Event *event);
void controller_handle_mouse_down(Controller *controller, SDL_Event *event);
void controller_handle_mouse_wheel(Controller *controller, SDL_Event *event);
void controller_handle_mouse_motion(Controller *controller, SDL_Event *event);
void controller_handle_resize(Controller *controller, SDL_Event *event);

/**
 * Destroys the controller, and underlying program model and view. 
 * 
 * This should be called after controller_thread() has returned, from a QUIT 
 * user input.
 * 
 * @param controller The controller to terminate.
 */
void controller_destroy(Controller *controller);

#endif // CONTROLLER_H
