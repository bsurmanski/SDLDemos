/**
 * noise.h
 * noise
 * November 14, 2011
 * Brandon Surmanski
 */

#ifndef _NOISE_H
#define _NOISE_H

void noise_seed(int s);
float noise_random1(int x);
float noise_value1(float x);
float noise_perlin1(float x);
float noise_fracPerlin1(float x, int n);
float noise_terbulence1(float x, int n);

float noise_random2(int x, int  y);
float noise_value2(float x, float y);
float noise_perlin2(float x, float y);
float noise_fracPerlin2(float x, float y, int n);
float noise_terbulence2(float x, float y, int n);

#endif
