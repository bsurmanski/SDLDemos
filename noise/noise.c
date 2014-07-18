/**
 * noise.c
 * noise
 * November 14, 2011
 * Brandon Surmanski
 */


#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "noise.h"

static int seed = 55;

/**
 * sets a seed for the noise functions to follow
 */
void noise_seed(int s)
{
    seed = s;
}

/**
 * linear interpolation between a and b at t%, where 't'
 * is a value between 0 and 1. 'a' is returned if 't' is 0,
 * and 'b' is returned if 't' is 1. A value inbetween 'a' and 'b'
 * is returned for values of t between 0 and 1.
 */
static inline float lerp(float t, float a, float b)
{
    return (a+t*(b-a));
}

/**
 * applys a smoothing function to the value of t. t must be between
 * 0 and 1. The value returned will be between 0 and 1, with the value
 * being slightly closer to the nearest integer(0/1). 
 */
static inline float s_curve(float t)
{
    return (t*t*t*(10+t*(-15+(6*t))));
}

/**
 * produces a random number from -1 to 1. The number is constant when the seed is the same
 */
inline float noise_random1(int x)
{
    x += seed;
    x = (x<<13) ^ x;
    double t = (1.0 - ((x*(x*x*15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
    return t;
}

/*
 * produces a smooth interpolated random number from -1 to 1. The value of integer
 * coordinates of 'x' will have a random value, and each fractional coordinate of 
 * x will be an interpolation of the closest integer values
 */
float noise_value1(float x)
{
    int flx = floor(x);
    float dx0 = noise_random1(flx);
    float dx1 = noise_random1(flx + 1);
    return lerp(x-flx, dx0, dx1);
}

/**
 * 1-dimensional perlion noise from -1 to 1.
 */
float noise_perlin1(float x)
{
    int flx = floor(x);
    float dx0 = noise_random1(flx);
    float dx1 = noise_random1(flx + 1);

    float u = (x-flx) * dx0;
    float v = ((x-flx)-1) * dx1;
    return lerp(s_curve(x-flx),u,v);
}

/**
 * 1-dimensional fractal perlin noise from -1 to 1. n 
 * defines the number of different 'octaves' to repeat
 * for the noise
 */
float noise_fracPerlin1(float x, int n)
{
    float sum = 0;
    int i;
    for(i = 1<<n; i>0; i = i>>1){
        sum += (noise_perlin1(x * i)/i);
    }
    return sum;
}

/**
 * terbulent noise. Perlin noise with discontinuous rates of change.
 * value between -1 and 1. n defines the number of 'octaves' to repeat
 */
float noise_terbulence1(float x, int n)
{
    float sum = 0;
    int i;
    for(i = 1<<n; i>0; i = i>>1)
        sum += fabs(noise_perlin1(x*i)/i);
    return sum * 2.0f - 1.0f;
}



/**
 * 2 Dimensional functions
 */

/*
 * simple vector normalization. if 'a' and 'b' are both 0,
 * an arbitarary value will be chosen.
 */
void normalize2(float *a, float *b)
{
    if(*a == 0 && *b == 0){
        *a = 0.7; *b = 0.3;
    }
    float s = 1.0f / sqrt((*a) * (*a) + (*b) * (*b));
    (*a) = (*a) * s;
    (*b) = (*b) * s;
}

/*
 * random value from 2 given seeds. See noise_random1 above
 */
inline float noise_random2(int x, int y)
{
    x += seed;
    y += seed;
    x += y * 71;
    x = (x << 13) ^ x;
    return (1.0f - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

/*
 * same as noise_random, except giving a different value for each given point
 */
inline float noise_random2b(int x, int y)
{
    x += seed;
    y += seed;
    x += y * 89;
    x = (x << 17) ^ x;
    return (1.0f - ((x * (x * x * 24421 + 544367) + 982451653) & 0x7fffffff) / 1073741824.0f);
}

/**
 * 2 dimensional value noise. See 1 dimensional value noise above.
 * quick noise function with the downside being rectangular artifacts
 */
float noise_value2(float x, float y)
{
    int flx = floor(x);
    int fly = floor(y);
    float v0 = noise_random2(flx, fly);
    float v1 = noise_random2(flx+1,fly);
    float v2 = noise_random2(flx+1,fly+1);
    float v3 = noise_random2(flx,fly+1);

    float px0 = lerp(s_curve(x-flx),v0,v1);
    float px1 = lerp(s_curve(x-flx),v3,v2);
    return lerp(s_curve(y-fly),px0,px1);
}

float noise_perlin2(float x, float y)
{
    int flx = floor(x);
    int fly = floor(y);
    float frx = x-flx;
    float fry = y-fly;

    float vx0 = noise_random2(flx,fly);
    float vy0 = noise_random2b(flx,fly);
    normalize2(&vx0, &vy0);
    float vx1 = noise_random2(flx+1.0f,fly);
    float vy1 = noise_random2b(flx+1.0f,fly);
    normalize2(&vx1, &vy1);
    float vx2 = noise_random2(flx+1.0f,fly+1.0f);
    float vy2 = noise_random2b(flx+1.0f,fly+1.0f);
    normalize2(&vx2, &vy2);
    float vx3 = noise_random2(flx,fly+1.0f);
    float vy3 = noise_random2b(flx,fly+1.0f);
    normalize2(&vx3, &vy3);

    float s = vx0 * (frx)        + vy0 * (fry);
    float t = vx1 * ((frx)-1.0f) + vy1 * (fry);
    float u = vx2 * ((frx)-1.0f) + vy2 * ((fry)-1.0f);
    float v = vx3 * (frx)        + vy3 * ((fry)-1.0f);

    float px0 = lerp(s_curve(frx), s, t);
    float px1 = lerp(s_curve(frx), v, u);

    return lerp(s_curve(fry), px0, px1);
}

float noise_fracPerlin2(float x, float y, int n)
{
    float sum = 0;
    int i;
    for(i = 1<<n; i>0; i = i>>1){
        sum += (noise_perlin2(x * i, y * i)/i);
    }
    return sum;
}

float noise_terbulence2(float x, float y, int n)
{
    float sum = 0;
    int i;
    for(i = 1<<n; i>0; i = i>>1)
        sum += fabs(noise_perlin2(x * i, y * i)/i);
    return sum * 2.0f - 1.0f;
}
