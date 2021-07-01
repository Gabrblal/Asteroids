#include "view/view_port.h"

#include "stdlib.h"

ViewPort *view_port_create(
    Vector2 position,
    Vector2 dimensions,
    Vector2 screen,
    double inertia,
    double max_velocity
) {
    ViewPort *view_port = malloc(sizeof(ViewPort));
    if (!view_port) {
        return NULL;
    }

    // Initially at specified position and 1.0 zoom.
    view_port->position = position;
    view_port->dimensions = dimensions;
    view_port->screen = screen;
    view_port->zoom = 1.0;

    // Initially not moving anywhere.
    view_port->movement.up = false;
    view_port->movement.down = false;
    view_port->movement.left = false;
    view_port->movement.right = false;

    // Initially has no velocity.
    view_port->movement.velocity_x = 0.0;
    view_port->movement.velocity_y = 0.0;
    view_port->movement.velocity_z = 0.0;
    view_port->movement.velocity_max = max_velocity;
    view_port->movement.velocity_max_zoom = 1.0;
    view_port->movement.inertia = inertia;

    return view_port;
}

void view_port_move_up(ViewPort *view_port, bool state) {
    view_port->movement.up = state;
}

void view_port_move_down(ViewPort *view_port, bool state) {
    view_port->movement.down = state;
}

void view_port_move_left(ViewPort *view_port, bool state) {
    view_port->movement.left = state;
}

void view_port_move_right(ViewPort *view_port, bool state) {
    view_port->movement.right = state;
}

void view_port_update(ViewPort *view_port)
{
    ViewPortMovement *m = &view_port->movement;

    if (m->left != m->right) {
        // Accelerate
        m->velocity_x += (m->right ? 1.0 : -1.0) / m->inertia;

        // Check bounds
        if (abs(m->velocity_x) > m->velocity_max) {
            m->velocity_x = (m->velocity_x > 0 ? 1.0 : -1.0) * m->velocity_max;
        }
    }
    else {
        // Deccelerate
        double dx = (m->velocity_x > 0 ? -1.0 : 1.0) / m->inertia;
        m->velocity_x += dx;

        // Check bounds
        if ((m->velocity_x > 0 && dx > 0) || (m->velocity_x < 0 && dx < 0)) {
            m->velocity_x = 0;
        }
    }

    if (m->up != m->down) {
        // Accelerate
        m->velocity_y += (m->down ? 1.0 : -1.0) / m->inertia;

        // Check bounds
        if (abs(m->velocity_y) > m->velocity_max) {
            m->velocity_y = (m->velocity_y > 0 ? 1.0 : -1.0) * m->velocity_max;
        }
    }
    else {
        // Deccelerate
        double dy = (m->velocity_y > 0 ? -1.0 : 1.0) / m->inertia;
        m->velocity_y += dy;

        // Check bounds
        if ((m->velocity_y > 0 && dy > 0) || (m->velocity_y < 0 && dy < 0)) {
            m->velocity_y = 0;
        }
    }

    view_port->position.x += m->velocity_x;
    view_port->position.y += m->velocity_y;
}

Vector2 view_port_to_world(ViewPort *view, Vector2 pixel)
{
    // Take the percentage that the pixel is across the screen and multiply that
    // by the maximum distance in meters across the screen. Add it to the 
    // position.
    Vector2 world = {
        view->position.x + pixel.x / view->screen.x * view->dimensions.x,
        view->position.y + pixel.y / view->screen.y * view->dimensions.y
    };
    return world;
}

Vector2 view_world_to_port(ViewPort *view, Vector2 coordinate)
{
    // Inverse of the above operation in view_port_to_world()
    Vector2 pixel = {
        (coordinate.x - view->position.x) * view->screen.x / view->dimensions.x,
        (coordinate.y - view->position.y) * view->screen.y / view->dimensions.x
    };
    return pixel;
}

void view_port_destroy(ViewPort *view_port)
{
    free(view_port);
}
