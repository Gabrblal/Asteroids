#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

/**
 * Generic two dimensional vector.
 */
typedef struct {
    double x; // First component.
    double y; // Second component.
} Vector;

/** 
 * Add two vectors elementwise.
 * 
 * @param a First vector.
 * @param b Second vector.
 * 
 * @return The elementwise sum of both vectors.
 */
static inline Vector vector_add(Vector a, Vector b)
{
    Vector c = {a.x + b.x, a.y + b.y};
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
static inline Vector vector_sub(Vector a, Vector b)
{
    Vector c = {a.x - b.x, a.y - b.y};
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
static inline Vector vector_mul(Vector a, Vector b)
{
    Vector c = {a.x * b.x, a.y * b.y};
    return c;
}

/** 
 * Scale a vector.
 * 
 * @param a First vector.
 * @param m The magnitude to scale by..
 * 
 * @return The scaled vector.
 */
static inline Vector vector_scale(Vector a, double m)
{
    Vector c = {a.x * m, a.y * m};
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
static inline Vector vector_div(Vector a, Vector b)
{
    Vector c = {a.x / b.x, a.y / b.y};
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
static inline double vector_dot(Vector a, Vector b)
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
static inline Vector vector_rot(Vector v, double theta)
{
    Vector rv = {
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
static inline double vector_mag(Vector v)
{
    return sqrt(pow(v.x, 2) + pow(v.y, 2)); 
}

/** 
 * Calculate unit vector.
 * 
 * @param v The vector to calculate the unit vector from.
 * @return The vectors unit vector.
 */
static inline Vector vector_unit(Vector v)
{
    double mag = vector_mag(v);
    Vector unit = {v.x / mag, v.y / mag};
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
static inline Vector vector_proj(Vector u, Vector v)
{
    double scalar = vector_dot(u, v) / pow(vector_mag(v), 2);
    Vector proj_v_u = {v.x * scalar, v.y * scalar};
    return proj_v_u;
}

/**
 * Calculate the vector perpendicular to v.
 * 
 * @param v The vector to get the perpendicular of.
 * @return The perpendicular vector.
 */
static inline Vector vector_perp(Vector v)
{
    Vector perp = {v.y, -v.x};
    return perp;
}

#endif // VECTOR_H
