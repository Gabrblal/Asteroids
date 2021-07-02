#include "view/view_port.h"

#include "stdlib.h"

ViewPort *view_port_create(
    Vector2 position,
    Vector2 dimensions,
    Vector2 screen,
    double acceleration,
    double velocity_max
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
    view_port->movement.in = false;
    view_port->movement.out = false;

    // Initially has no velocity.
    view_port->movement.v_x = 0.0;
    view_port->movement.v_y = 0.0;
    view_port->movement.v_z = 1.0;
    view_port->movement.v_max = velocity_max;
    view_port->movement.a = acceleration;

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

void view_port_move_in(ViewPort *view_port, bool state) {
    view_port->movement.in = state;
}

void view_port_move_out(ViewPort *view_port, bool state) {
    view_port->movement.out = state;
}

void view_port_update(ViewPort *view_port)
{
    /// @todo: This code is essentially repeated 3 times, create a function
    /// to reduce bloat.

    /// @todo: Rewrite with control systems style reference velocity and PID
    /// control.

    ViewPortMovement *m = &view_port->movement;

    if (m->left != m->right) {
        // Accelerate
        m->v_x += (m->right ? m->a : -m->a) * view_port->dimensions.x;

        // Check bounds
        if (abs(m->v_x) > m->v_max * view_port->dimensions.x) {
            m->v_x = m->v_x > 0 ? m->v_max : -m->v_max;
        }
    }
    else if (m->v_x != 0) {
        // Deccelerate
        double dx = m->v_x > 0 ? -m->a : m->a;
        m->v_x += dx;

        // Check bounds
        if ((m->v_x > 0 && dx > 0) || (m->v_x < 0 && dx < 0)) {
            m->v_x = 0;
        }
    }

    if (m->up != m->down) {
        // Accelerate
        m->v_y += (m->down ? m->a : -m->a) * view_port->dimensions.y;

        // Check bounds
        if (abs(m->v_y) > m->v_max * view_port->dimensions.y) {
            m->v_y = m->v_y > 0 ?  m->v_max : -m->v_max;
        }
    }
    else if (m->v_y != 0) {
        // Deccelerate
        double dy = m->v_y > 0 ? -m->a : m->a;
        m->v_y += dy;

        // Check bounds
        if ((m->v_y > 0 && dy > 0) || (m->v_y < 0 && dy < 0)) {
            m->v_y = 0;
        }
    }

    if (m->in != m->out) {
        // Accelerate
        m->v_z += m->in ? m->a : -m->a;

        // Check bounds
        if (abs(m->v_z) > 1.0 + m->v_max) {
            m->v_z = m->v_z > 0 ?  m->v_max : -m->v_max;
        }
    }
    else if (m->v_z != 1.0) {
        // Deccelerate
        double dz = m->v_z > 1.0 ? -m->a : m->a;
        m->v_z += dz;

        // Check bounds
        if ((m->v_z < 1.0 && dz < 0) || (m->v_z > 1.0 && dz > 0)) {
            m->v_z = 1.0;
        }
    }

    view_port->position.x += m->v_x;
    view_port->position.y += m->v_y;
    view_port->dimensions.x *= m->v_z;
    view_port->dimensions.y *= m->v_z;
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
