#ifndef ASTEROID_H
#define ASTEROID_H

#include "model/object.h"
#include "model/polygon.h"

typedef struct {
    Object *object;
    Array *polygon;
    Array *verticies;
} Asteroid;

/**
 * @brief Create a new asteroid.
 */
Asteroid *asteroid_create();

/**
 * Advance the position of the asteroid.
 * 
 * @param asteroid The asteroid to advance.
 */
void asteroid_advance(Asteroid *asteroid, double seconds);

/**
 * @brief Deallocate memory previously allocated for an asteroid. Using the
 * asteroid after this call is undefined.
 * 
 * @param asteroid The asteroid to destroy.
 */
void asteroid_destroy(Asteroid *asteroid);

#endif // ASTEROID_H
