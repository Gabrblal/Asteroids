#include "model/asteroid.h"

Asteroid *asteroid_create()
{
    Asteroid *asteroid = malloc(sizeof(Asteroid));
    if (!asteroid)
        return NULL;

    asteroid->object = object_create();
    asteroid->polygon = polygon_create_random_regular(1);
    asteroid->verticies = array_create_from_array(asteroid->polygon);

    return asteroid;
}

void asteroid_advance(Asteroid *asteroid, double seconds)
{
    object_advance(asteroid->object, seconds);

    Vector *verticies = array_data(asteroid->polygon);
    Vector *coordinates = array_data(asteroid->verticies);
    int n = array_length(asteroid->verticies);

    // Calculate the new verticies. Rotate by the current angle and add the
    // position offset.
    for (int i = 0; i < n; i++) {
        *(coordinates + i) = vector_add(
            vector_rot(*(verticies + i), asteroid->object->angle),
            asteroid->object->position
        );
    }
}

void asteroid_destroy(Asteroid *asteroid)
{
    if (!asteroid)
        return;

    object_destroy(asteroid->object);
    array_destroy(asteroid->polygon);
    array_destroy(asteroid->verticies);

    free(asteroid);
}
