#ifndef VIEW_PORT_H
#define VIEW_PORT_H

#include <stdbool.h>

#include "util/vector2.h"
#include "util/time.h"

/**
 * Description of how the view port is moving, is it moving up or down, left or
 * right, in or out?
 * 
 * A view port has some velocity that describes how fast it is moving, and an
 * inertia to make it keep moving for a bit when it is no longer being moved to
 * make it look smooth.
 */
typedef struct ViewPortMovement ViewPortMovement;
struct ViewPortMovement {
    // Is the port moving up?
    bool up;
    // Is the port moving down?
    bool down;
    // Is the port moving left?
    bool left;
    // Is the port moving right?
    bool right;
    // Is the port moving zooming in?
    bool in;
    // Is the port moving zooming out?
    bool out;
    // Velocity left or right, in percentage of the ViewPort dimensions.
    double v_x;
    // Velocity up or down, in percentage of the ViewPort dimensions.
    double v_y;
    // Velocity of the zoom in the z direction, in percentage of the ViewPort 
    // dimensions.
    double v_z;
    // How fast or slow the view should accelerate.
    double a;
    // Maximum panning velocity, in percentage of the ViewPort dimensions.
    double v_max;
};

/**
 * A view port is a region of world space that the user is currently viewing.
 * We define this as a coordinate that the user is at, and a space (width and
 * height) around that point that can be seen.
 */
typedef struct ViewPort ViewPort;
struct ViewPort {
    // The position of the view port in world space (meters).
    Vector2 position;
    // The radius of the rectangular region of space seen in the world
    // (meters). That is half the width and half the height of the view port.
    Vector2 dimensions;
    // The dimensions of the screen.
    Vector2 screen;
    // Zoom of the view port relative to the initial state.
    double zoom;
    // The current movement of the view port.
    ViewPortMovement movement;
    // Time the view port was last updated.
    Time time;
};

/**
 * Create a new ViewPort viewing the region of world space described by position
 * and dimensions.
 * 
 * @param position Where the view port should be located initially.;
 * @param dimensions Half the rectangular dimensions of the area seen by the view.
 * @param screen The screen dimensions in pixels.
 * @param acceleration The view's acceleration as a percentage of it's
 * dimensions.
 * @param max_velocity The maximum speed of the view as a percentage of it's
 * dimensions.
 * @return A pointer to the new ViewPort instance.
 */
ViewPort *view_port_create(
    Vector2 position,
    Vector2 dimensions,
    Vector2 screen,
    double acceleration,
    double max_velocity
);

/**
 * Toggle the view port moving up.
 */
void view_port_move_up(ViewPort *view_port, bool state);

/**
 * Toggle the view port moving down.
 */
void view_port_move_down(ViewPort *view_port, bool state);

/**
 * Toggle the view port moving left.
 */
void view_port_move_left(ViewPort *view_port, bool state);

/**
 * Toggle the view port moving right.
 */
void view_port_move_right(ViewPort *view_port, bool state);

/**
 * Toggle the view port moving left.
 */
void view_port_move_in(ViewPort *view_port, bool state);

/**
 * Toggle the view port moving right.
 */
void view_port_move_out(ViewPort *view_port, bool state);

/**
 * Update a view port based on if it is being moved along any of the axes, and
 * using the current velocity and inertia to determine the current position.
 */
void view_port_update(ViewPort *view);

/**
 * Transform a pixel on the screen to a world coordinate.
 * 
 * @param pixel The screen pixel location.
 * 
 * @return The world coordinate of the pixel.
 */
Vector2 view_port_to_world(ViewPort *view, Vector2 pixel);

/**
 * Transform a world coordinate to a pixel on the screen.
 * 
 * @param coordinate The world coordinate.
 * 
 * @return The pixel on the screen for the world coordinate.
 */
Vector2 view_world_to_port(ViewPort *view, Vector2 coordinate);

/**
 * Destroy a view port. Using the view port after this function is called on
 * it is undefined.
 * 
 * @param view_port The view port to destroy.
 */
void view_port_destroy(ViewPort *view_port);

#endif // VIEW_PORT_H