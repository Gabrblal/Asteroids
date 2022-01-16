#ifndef POLYGON_H
#define POLYGON_H

#include <stdbool.h>

#include "util/array.h"

/**
 * A polygon is simply an array of Vector coordinates, such that the last
 * coordinate wraps around the the first coordinate. Ordered pairs of points in
 * the array define the line segments of the polygon.
 */
Array *polygon_create();

/**
 * @brief Create a random regular polygon.
 * 
 * @returns Pointer to the new polygon, being an Array of Vector.
 */
Array *polygon_create_random_regular(double radius);

/**
 * @brief Determine whether two polygons are colliding using the seperating axis
 * theorem.
 * 
 * @param a The first polygon.
 * @param b The second polygon.
 * @param colliding The bool to set to true if a and b are colliding, otherwise
 * set to false.
 * 
 * @returns True if the algorithm succeeded, otherwise false.
 */
bool polygon_colliding(Array *A, Array *B, bool *colliding);

#endif // POLYGON_H
