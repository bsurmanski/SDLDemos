/**
 * sdl.h
 * earth
 * November 6, 2012
 * Brandon Surmanski
 */

void *sdl_pixelAt(SDL_Surface *s, int x, int y);
void sdl_setPixel(SDL_Surface *s, int x, int y, int val);
void sdl_surfaceCopy(SDL_Surface *dst, SDL_Surface *src);
