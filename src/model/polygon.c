#include "model/polygon.h"

#include <stdlib.h>
#include <stdio.h>

#include "util/random.h"
#include "util/vector.h"

Array *polygon_create()
{
    return array_create(sizeof(Vector));
}

Array *polygon_create_random_regular(double radius)
{
    Array *polygon = polygon_create();
    if (!polygon)
        return NULL;

    // The number of verticies.
    int n = random_int(3, 10);

    // The angle between each vertex.
    double d = 2 * M_PI / n;

    // Loop through all the verticies starting on the x axis and append them to
    // the polygon.
    double angle = 0;
    for (int i = 0; i < n; i++) {
        Vector vertex = {radius * cos(angle), radius * sin(angle)};
        array_push_back(polygon, &vertex);
        angle += d;
    }

    return polygon;
}

bool polygon_axes_shadow_overlap(Vector *A, Vector *B, int N, int M)
{
    // Get the edge coordinates of polygon A.
    Vector a = *A;
    for (int i = 0; i < N; i++) {
        Vector b = *(A + i + 1);

        // Calculate the seperating axis as the vector perpendicular to the
        // edge of polygon A.
        Vector axis = vector_unit(vector_perp(vector_sub(b, a)));

        // The shadow of polygon A.
        double A_max = vector_dot(axis, *A);
        double A_min = A_max;
        for (int j = 1; j < N; j++) {
            double projection = vector_dot(axis, *(A + j));
            if (projection > A_max)
                A_max = projection;
            else if (projection < A_min)
                A_min = projection;
        }

        // The shadow of polygon B.
        double B_max = vector_dot(axis, *B);
        double B_min = B_max;
        for (int j = 1; j < N; j++) {
            double projection = vector_dot(axis, *(B + j));
            if (projection > B_max)
                B_max = projection;
            else if (projection < B_min)
                B_min = projection;
        }

        if (B_min > A_max || B_max < A_min)
            return false;

        a = b;
    }

    return true;
}

bool polygon_colliding(Array *polygon_A, Array *polygon_B, bool *colliding)
{
    // This algorithm takes the line segments of polygon A, and projects
    // the shadow of polygon B onto that line. If the shadow of B doesn't
    // overlap with the line segment of A, then the polygons are not colliding.
    // Otherwise we continue to loop through the line segments of polygon A.
    // If we cannot find a seperating axis, then the polygons are colliding.

    // For performance, get the raw data buffers of each polygon.
    Vector *A = array_data(polygon_A);
    Vector *B = array_data(polygon_B);

    // Get the length of the data buffers.
    int N = array_length(polygon_A);
    int M = array_length(polygon_B);

    // Require the polygons to have at least 3 coordinates.
    if (N < 3 || M < 3)
        return false;

    *colliding = (
        polygon_axes_shadow_overlap(A, B, N, M) &&
        polygon_axes_shadow_overlap(B, A, N, M)
    );

    return true;
}
