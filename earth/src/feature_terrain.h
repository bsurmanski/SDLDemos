/**
 * feature_terrain.h
 * earth
 * November 08, 2012
 * Brandon Surmanski
 */

#ifndef _FEATURE_TERRAIN_H
#define _FEATURE_TERRAIN_H

#include "feature.h"

void feature_terrain_create(Feature *f);
void feature_terrain_step(Feature *f, float yrs);

#endif
