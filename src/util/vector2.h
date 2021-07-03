#ifndef VECTOR2_H
#define VECTOR2_H

#include <math.h>

typedef struct {
    double x;
    double y;
} Vector2;

/** 
 * Adds two vectors.
 * 
 * @param a first vector
 * @param b second vector
 */
static inline Vector2 vector2_add(Vector2 a, Vector2 b)
{
    Vector2 c = {a.x + b.x, a.y + b.y};
    return c;
}

/** 
 * Subtracts two vectors.
 * 
 * @param a first vector
 * @param b second vector
 */
static inline Vector2 vector2_sub(Vector2 a, Vector2 b)
{
    Vector2 c = {a.x - b.x, a.y - b.y};
    return c;
}

/** 
 * Multiplies two vectors.
 * 
 * @param a first vector
 * @param b second vector
 */
static inline Vector2 vector2_mul(Vector2 a, Vector2 b)
{
    Vector2 c = {a.x * b.x, a.y * b.y};
    return c;
}

/** 
 * Divides two vectors.
 * 
 * @param a first vector
 * @param b second vector
 */
static inline Vector2 vector2_div(Vector2 a, Vector2 b)
{
    Vector2 c = {a.x / b.x, a.y / b.y};
    return c;
}

/** 
 * Calculates dot product of two vectors.
 * 
 * @param a first vector
 * @param b second vector
 */
static inline double vector2_dot(Vector2 a, Vector2 b)
{
    return a.x * b.x + a.y * b.y;
}

/** 
 * Rotates a vector.
 * 
 * @param v
 * @param theta radians to rotate by
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
 * @param v
 */
static inline double vector2_mag(Vector2 v)
{
    return sqrt(pow(v.x, 2) + pow(v.y, 2)); 
}

/** 
 * Creates unit vector.
 * 
 * @param v
 */
static inline Vector2 vector2_unit(Vector2 v)
{
    double mag = vector2_mag(v);
    Vector2 unit = {v.x / mag, v.y / mag};
    return unit;
}

/** 
 * Creates projection of V on U
 * 
 * @param v
 * @param u
 */
static inline Vector2 vector2_proj(Vector2 u, Vector2 v)
{
    double scalar = vector2_dot(u, v) / pow(vector2_mag(v), 2);
    Vector2 proj_v_u = {v.x * scalar, v.y * scalar};
    return proj_v_u;
}

#endif // VECTOR2_H
