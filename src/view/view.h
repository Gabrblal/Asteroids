#ifndef VIEW_H
#define VIEW_H

#include "SDL2/SDL.h"

#include "util/definitions.h"
#include "util/intervalthread.h"
#include "view/view_port.h"

/**
 * Represents the graphical component of the application, keeping track of the
 * renderering thread, user view of the world space (moving the user's view
 * around) and drawing the model to the current view.
 */
typedef struct View View;
struct View {
    // Main game window, used for making the renderer.
    SDL_Window *window;
    // Renderer used to clear, draw and render the screen.
    SDL_Renderer *renderer;
    // The current view area of the model.
    ViewPort *port;
    // Thread to update the screen with intermittently.
    IntervalThread *thread;
    // Mutex protecting concurrent access of view data.
    SDL_mutex *mutex;
    // Function that can be binded to draw onto the window. Takes this view to
    // draw onto and a void* as the drawn data.
    void(*draw_function)(View*, void*);
    // Data to pass to draw_function.
    void *data;
    // Timer that calls a callback to print how many frames have been rednered
    // in the last second, and reset the fps counter.
    SDL_TimerID fps_counter_timer;
    // Frames per second, locked under View->mutex.
    int fps;
};

/**
 * Create a view of the game, that periodically accesses the model and uses the 
 * retrieved data to draw to the window.
 * 
 * @param window Pointer to the window to draw onto.
 * @param port The initial view port to use.
 * @param draw_function Function to call to draw onto the view. Takes a view
 * object to draw onto, and a void * to pass the drawn data.
 * @param data The data to pass to draw_function.
 * 
 * @return A pointer to the view, to use with the rest of the view interface.
 */
View *view_create(
    SDL_Window *window,
    ViewPort *port,
    void(*draw_function)(View*, void*),
    void *data
);

/**
 * Thread function that runs to clear, draw and render to the view window. 
 * 
 * Draws the model to the window.
 * 
 * @param data Pointer to buffer (View object) to pass to the thread function.
 */
void view_draw(void *data);

/**
 * Callback to pass to an internal SDL timer (that calls this each second) to
 * calculate the number of frames per second since the last call.
 */
uint32_t view_fps_counter_callback(uint32_t interval, void *data);

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
 * Set the position of the view to the coordinate pos.
 * 
 * @param view Pointer to the view to set the position.
 * @param pos The coordinate to set the position to.
 */
void view_set_position(View *view, Vector pos);

/**
 * Draw a grid in the world space.
 * 
 * @param view The view to which to draw the grid.
 */
void view_draw_grid(View *view);

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
