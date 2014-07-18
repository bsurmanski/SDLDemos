/**
 * sdl.c
 * earth
 * November 06, 2012
 * Brandon Surmanski
 */

#include <SDL/SDL.h>

#include <stdlib.h>

#include "sdl.h"

void *sdl_pixelAt(SDL_Surface *s, int x, int y)
{
    return (((char*) s->pixels) + x * s->format->BytesPerPixel + y * s->pitch);
}

void sdl_setPixel(SDL_Surface *s, int x, int y, int val)
{
    memcpy(sdl_pixelAt(s,x,y), &val, s->format->BytesPerPixel);
}

void sdl_surfaceCopy(SDL_Surface *dst, SDL_Surface *src)
{
    memcpy(dst->pixels, src->pixels, dst->pitch * dst->h);
}
