/**
 * vec.c
 * noise
 * November 14, 2011
 * Brandon Surmanski
 */

#include "vec.h"

float vec2_dot(vec2 *a, vec2 *b)
{
    return a->x * b->x + a->y * b->y;
}

float vec3_dot(vec3 *a, vec3 *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}
