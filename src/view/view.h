#ifndef VIEW_H
#define VIEW_H

#include "SDL2/SDL.h"
#include "spin/spin.h"
#include "util/definitions.h"
#include "util/vector2.h"

/**
 * Represents the graphical component of the application, keeping track of the
 * renderering thread, user view of the world space (moving the user's view
 * around) and drawing the model to the current view.
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
 * Thread function that runs to clear, draw and render to the view window. 
 * 
 * Draws the model to the window.
 * 
 * @param data Pointer to buffer (View object) to pass to the thread function.
 */
int view_thread(void *data);

/**
 * Signals the view thread to draw onto the window, if it is waiting to draw.
 * 
 * @param view The view to signal to draw.
 */
void view_notify(View *view);

/**
 * Checks if the current view has exited. 
 * 
 * Thread safe. 
 * 
 * @return If the view should be exited.
 */
bool view_done(View *view);

/**
 * Resize the window controlled by the view.
 * 
 * The new width and height must be greater than zero.
 * 
 * @param view The view whose window to resize.
 * @param x The new width of the view.
 * @param y The new height of the view.
 */
void view_resize_window(View *view, int x, int y);

/**
 * Toggle the movement of the view in a given direction on or off.
 * 
 * @param view The view to move.
 * @param direction The direction to move the view.
 * @param state The state of that direction, either on or off.
 */
void view_move(View *view, Direction direction, bool state);

/**
 * Draw a spin object to a renderer.
 * 
 * @param renderer The renderer to draw the spin model with.
 * @param spin The spin object to draw.
 */
void spin_view_update(View *view, SpinData *spin);

/**
 * Terminates the running View thread, destroys view contents and deallocates
 * the view. 
 * 
 * Using the view after calling this function is undefined.
 * 
 * @param view The view instance to destroy.
 */
void view_destroy(View *view);

#endif // VIEW_H