/**
 * vec.h
 * noise
 * November 11, 2011
 * Brandon Surmanski
 */

#ifndef _VEC_H
#define _VEC_H

typedef struct vec2 {
    float x;
    float y;
} vec2;

typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

float vec2_dot(vec2 *a, vec2 *b);

float vec3_dot(vec3 *a, vec3 *b);

#endif
