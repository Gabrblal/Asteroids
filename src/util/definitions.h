#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/**
 * Generic direction definition in 3D space along the NORTH-EAST, 
 * SOUTH-WEST and IN-OUT axes.
 */
typedef enum {
    DIRECTION_NONE,
    DIRECTION_NORTH,
    DIRECTION_EAST,
    DIRECTION_SOUTH,
    DIRECTION_WEST,
    DIRECTION_IN,
    DIRECTION_OUT
} Direction;

#endif // DEFINITIONS_H