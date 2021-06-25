#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "SDL2/SDL.h"

/**
 * Initialises SDL and game model.
 */
void controller_init();

void controller_handle_events();
void controller_handle_key_up(SDL_Event *event);
void controller_handle_key_down(SDL_Event *event);
void controller_handle_mouse_up(SDL_Event *event);
void controller_handle_mouse_down(SDL_Event *event);
void controller_handle_mouse_wheel(SDL_Event *event);
void controller_handle_mouse_motion(SDL_Event *event);

/**
 * Begins the application.
 */
void controller_main();

#endif // CONTROLLER_H