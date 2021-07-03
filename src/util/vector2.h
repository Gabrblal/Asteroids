#ifndef VECTOR2_H
#define VECTOR2_H

#include <math.h>

/**
 * Generic two dimensional vector.
 */
typedef struct {
    double x; // First component.
    double y; // Second component.
} Vector2;

/** 
 * Add two vectors elementwise.
 * 
 * @param a First vector.
 * @param b Second vector.
 * 
 * @return The elementwise sum of both vectors.
 */
static inline Vector2 vector2_add(Vector2 a, Vector2 b)
{
    Vector2 c = {a.x + b.x, a.y + b.y};
    return c;
}

/** 
 * Subtract two vectors.
 * 
 * @param a First vector.
 * @param b Second vector.
 * 
 * @return The element-wise subtraction of a from b (a - b).
 */
static inline Vector2 vector2_sub(Vector2 a, Vector2 b)
{
    Vector2 c = {a.x - b.x, a.y - b.y};
    return c;
}

/** 
 * Multiply two vectors.
 * 
 * @param a First vector.
 * @param b Second vector.
 * 
 * @return The element-wise multiplication of both vectors.
 */
static inline Vector2 vector2_mul(Vector2 a, Vector2 b)
{
    Vector2 c = {a.x * b.x, a.y * b.y};
    return c;
}

/** 
 * Divide two vectors.
 * 
 * @param a First vector.
 * @param b Second vector.
 * 
 * @return The elementwise quotient of a and b (a / b).
 */
static inline Vector2 vector2_div(Vector2 a, Vector2 b)
{
    Vector2 c = {a.x / b.x, a.y / b.y};
    return c;
}

/** 
 * Calculates the dot product of two vectors.
 * 
 * @param a First vector.
 * @param b Second vector.
 * 
 * @return The dot product (a.b).
 */
static inline double vector2_dot(Vector2 a, Vector2 b)
{
    return a.x * b.x + a.y * b.y;
}

/** 
 * Rotates a vector.
 * 
 * @param v The vector to rotate.
 * @param theta Radians to rotate by.
 * 
 * @return The rotated vector.
 */
static inline Vector2 vector2_rot(Vector2 v, double theta)
{
    Vector2 rv = {
        v.x * cos(theta) - v.y * sin(theta),
        v.x * sin(theta) + v.y * cos(theta)
    };
    return rv;
}

/** 
 * Returns vector magnitude.
 * 
 * @param v The vector to determine the magnitude of.
 * @return The vectors magnitude.
 */
static inline double vector2_mag(Vector2 v)
{
    return sqrt(pow(v.x, 2) + pow(v.y, 2)); 
}

/** 
 * Calculate unit vector.
 * 
 * @param v The vector to calculate the unit vector from.
 * @return The vectors unit vector.
 */
static inline Vector2 vector2_unit(Vector2 v)
{
    double mag = vector2_mag(v);
    Vector2 unit = {v.x / mag, v.y / mag};
    return unit;
}

/** 
 * Projection of a vector u onto a vector v.
 * 
 * @param u Vector to project from.
 * @param v Vector to project to.
 * 
 * @return Scalar of vector v that u is projected onto.
 */
static inline Vector2 vector2_proj(Vector2 u, Vector2 v)
{
    double scalar = vector2_dot(u, v) / pow(vector2_mag(v), 2);
    Vector2 proj_v_u = {v.x * scalar, v.y * scalar};
    return proj_v_u;
}

#endif // VECTOR2_H
