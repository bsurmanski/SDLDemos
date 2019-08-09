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

static int seed = 113;

float lerp(float t, float a, float b)
{
    return (a+t*(b-a));
}

float s_curve(float t)
{
    return (t*t*t*(10+t*(-15+(6*t))));
}

float noise_random1(int x)
{
    x += seed;
    x = (x<<13) ^ x;
    double t = (1.0 - ((x*(x*x*15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
    return t;
}

float noise_value1(float x)
{
    int flx = floor(x);
    float dx0 = noise_random1(flx);
    float dx1 = noise_random1(flx + 1);
    return lerp(s_curve(x-flx), dx0, dx1);
}

float noise_perlin1(float x)
{
    int flx = floor(x);
    float dx0 = noise_random1(flx);
    float dx1 = noise_random1(flx + 1);

    float u = (x-flx) * dx0;
    float v = ((x-flx)-1) * dx1;
    return lerp(s_curve(x-flx),u,v);
}

float noise_fracPerlin1(float x, int n)
{
    float sum = 0;
    int i;
    for(i = 1<<n; i>0; i = i>>1){
        sum += (noise_perlin1(x * i)/i);
    }
    return sum;
}

float noise_terbulence1(float x, int n)
{
    float sum = 0;
    int i;
    for(i = 1<<n; i>0; i = i>>1)
        sum += fabs(noise_perlin1(x*i)/i);
    return sum;
}

/**
 * 2 Dimensional functions
 */

void normalize2(float *a, float *b)
{
    if(*a == 0 && *b == 0){
        *a = 0.7; *b = 0.3;
    }
    float s = sqrt((*a) * (*a) + (*b) * (*b));
    (*a) = (*a) / s;
    (*b) = (*b) / s;
}

float noise_random2(int x, int y)
{
    x += seed;
    y += seed;
    x += y * 71;
    x = (x << 13) ^ x;
    return (1.0f - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824);
}

float noise_random2b(int x, int y)
{
    x += seed;
    y += seed;
    x += y * 89;
    x = (x << 17) ^ x;
    return (1.0f - ((x * (x * x * 24421 + 544367) + 982451653) & 0x7fffffff) / 1073741824);
}

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

    float vx0 = noise_random2(flx,fly);
    float vy0 = noise_random2b(flx,fly);
    normalize2(&vx0, &vy0);
    float vx1 = noise_random2(flx+1,fly);
    float vy1 = noise_random2b(flx+1,fly);
    normalize2(&vx1, &vy1);
    float vx2 = noise_random2(flx+1,fly+1);
    float vy2 = noise_random2b(flx+1,fly+1);
    normalize2(&vx2, &vy2);
    float vx3 = noise_random2(flx,fly+1);
    float vy3 = noise_random2b(flx,fly+1);
    normalize2(&vx3, &vy3);

    float s = vx0 * (x-flx)     + vy0 * (y-fly);
    float t = vx1 * ((x-flx)-1) + vy1 * (y-fly);
    float u = vx2 * ((x-flx)-1) + vy2 * ((y-fly)-1);
    float v = vx3 * (x-flx)     + vy3 * ((y-fly)-1);

    float px0 = lerp(x-flx, s, t);
    float px1 = lerp(x-flx, v, u);

    return lerp(s_curve(y-fly), px0, px1);
}
