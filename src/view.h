#ifndef VIEW_H
#define VIEW_H

#include <stdbool.h>

#include "controller.h"
#include "SDL2/SDL.h"
#include "spin/spin.h"

/**
 * Struct containing view related data.
 */
typedef struct View View;

/**
 * Create a view of the game, that periodically accesses the model and uses the
 * retrieved data to draw to the window.
 * 
 * @param window Pointer to the window to draw onto.
 * @param spin Spin model to draw.
 * 
 * @return A pointer to the view, to use with the rest of the view interface.
 */
View *view_create(SDL_Window *window, Spin *spin);

/**
 * Notifies the view that it should draw everything.
 * 
 * @param view The view to redraw.
 */
void view_notify(View *view);

/**
 * Deallocates a view.
 */
void view_destroy(View *view);

#endif // VIEW_H