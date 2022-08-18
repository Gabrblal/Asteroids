#ifndef OBJECT_H
#define OBJECT_H

#include "util/vector.h"

typedef struct {
    double mass;
    Vector position;
    Vector velocity;
    Vector acceleration;
    double angle;
    double omega;
    double alpha;
} Object;

/**
 * @brief Create a defaultly initalised object.
 * 
 * @returns A pointer to the object.
 */
Object *object_create();

/**
 * @brief Update the attributes of an object after a given number of 
 * seconds has elapsed.
 * 
 * @param seconds The number of seconds that have elapsed.
 */
void object_advance(Object *object, double seconds);

/**
 * @brief Deallocates memory allocated for the provided object. Using the object
 * after this call is undefined.
 * 
 * @param object The object to destroy.
 */
void object_destroy(Object *object);

#endif // OBJECT_H
