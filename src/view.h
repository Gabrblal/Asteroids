#ifndef VIEW_H
#define VIEW_H

#include "SDL2/SDL.h"
#include "spin/spin.h"
#include "util/vector2.h"

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
 * Checks if the current view has exited. 
 * 
 * Thread safe. 
 * 
 * @return If the view should be exited.
 */
bool view_done(View *view);

/**
 * Signals the view to draw onto the window, if it is waiting to draw.
 * 
 * @param view The view to signal to draw.
 */
void view_notify(View *view);

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
 * Draw a spin object to a renderer.
 * 
 * @param renderer The renderer to draw the spin model with.
 * @param spin The spin object to draw.
 */
void spin_view_update(View *view, SpinData *spin);

/**
 * Transform a pixel on the screen to a world coordinate.
 * 
 * @param pixel The screen pixel location.
 * 
 * @return The world coordinate of the pixel.
 */
Vector2 view_port_to_world(View *view, Vector2 pixel);

/**
 * Transform a world coordinate to a pixel on the screen.
 * 
 * @param coordinate The world coordinate.
 * 
 * @return The pixel on the screen for the world coordinate.
 */
Vector2 view_world_to_port(View *view, Vector2 coordinate);

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