/**
 * feature_sdl.h
 * earth
 * November 08, 2012
 * Brandon Surmanski
 */

#ifndef _FEATURE_SDL_H
#define _FEATURE_SDL_H

#include <SDL/SDL.h>

#include "clockwork/util/math/rect.h"

#include "feature.h"

void feature_sdl_blit(Feature *f, SDL_Surface *s, irect2 src, irect2 dst);

#endif
