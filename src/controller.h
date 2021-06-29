#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdbool.h>

typedef struct Controller Controller;

/**
 * Creates a new controller instance, starting the model and model view.
 * 
 * Must be called in main thread.
 * 
 * @return Main Controller struct containing main program data.
 */
Controller *controller_create();

void controller_thread(Controller *controller);

/**
 * Terminates the controller program.
 * 
 * @param controller The controller to terminate.
 */
void controller_destroy(Controller *controller);

/**
 * Checks if the controller is exiting.
 * 
 * @return If the controller has finished executing.
 */
bool controller_done(Controller *controller);

#endif // CONTROLLER_H