/**
 * feature_terrain.c
 * earth
 * November 08, 2012
 * Brandon Surmanski
 */

#include <stdio.h>

#include "clockwork/util/noise.h"
#include "clockwork/util/math/scalar.h"

#include "feature.h"
#include "feature_terrain.h"

void feature_terrain_create(Feature *f)
{
    int i,j;
    for(j = 0; j < f->h; j++)
    {
        for(i = 0; i < f->w; i++)
        {
            float perlin = 
                (noise2_fracPerlin(i / 100.0f, j / 100.0f, 5) * (f->w - i) * (f->h - j) +
                noise2_fracPerlin((i - f->w) / 100.0f, j / 100.0f, 5) * (i) * (f->h - j) +
                noise2_fracPerlin((i - f->w) / 100.0f, (j - f->h) / 100.0f, 5) * (i) * (j) +
                noise2_fracPerlin(i / 100.0f, (j - f->h) / 100.0f, 5) * (f->w - i) * (j))
                / (f->w * f->h);
            uint16_t val = ftouh((perlin / 8.0f + 1)/ 2.0f);
            feature_set(f, i, j, val);
        }
    }
}
