/**
 * feature.c
 * earth
 * November 07, 2012
 * Brandon Surmanski
 */

#include <stdlib.h>

#include "clockwork/util/math/scalar.h"

#include "feature.h"

void feature_init(Feature *f, int w, int h)
{
    f->w = w;
    f->h = h;
    f->arr = malloc(sizeof(uint16_t) * w * h);
}

void feature_finalize(Feature *f)
{
    free(f->arr);
}

void feature_set(Feature *f, int x, int y, uint16_t val)
{
    f->arr[f->w * y + x] = val;
}

uint16_t feature_get(Feature *f, int x, int y)
{
    int idx = iwrap(f->w * y + x, f->w * f->h);
    return f->arr[idx];
}
