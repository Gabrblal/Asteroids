#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/**
 * Generic direction definition in 3D space along the NORTH-EAST, 
 * SOUTH-WEST and IN-OUT axes.
 */
typedef enum Direction Direction;
enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    IN,
    OUT
};

#endif // DEFINITIONS_H