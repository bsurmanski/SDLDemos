/**
 * feature.h
 * earth
 * November 07, 2012
 * Brandon Surmanski
 */

#ifndef _FEATURE_H
#define _FEATURE_H

#include <stdint.h>

typedef struct Feature
{
    uint16_t w;
    uint16_t h;
    uint8_t PADDING[4];
    uint16_t *arr;
} Feature;

void feature_init(Feature *f, int w, int h);
void feature_finalize(Feature *f);
void feature_set(Feature *f, int x, int y, uint16_t val);
uint16_t feature_get(Feature *f, int x, int y);

#endif
