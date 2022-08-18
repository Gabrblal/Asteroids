#include "model/object.h"

#include <stdlib.h>

Object *object_create()
{
    Object *object = malloc(sizeof(Object));
    if (!object)
        return NULL;

    object->mass = 0.0;

    // Position.
    object->position = (Vector){0.0, 0.0};
    object->velocity = (Vector){0.0, 0.0};
    object->acceleration = (Vector){0.0, 0.0};

    // Rotation
    object->angle = 0.0;
    object->omega = 0.0;
    object->alpha = 0.0;

    return object;
}

void object_advance(Object *object, double seconds)
{
    object->velocity = vector_add(object->velocity, object->acceleration);
    object->position = vector_add(object->position, object->velocity);

    object->omega += object->alpha;
    object->angle += object->omega;
}

void object_destroy(Object *object)
{
    if (!object)
        return;

    free(object);
}