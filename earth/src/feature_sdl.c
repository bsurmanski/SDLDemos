/**
 * feature_sdl.c
 * earth
 * November 08, 2012
 * Brandon Surmanski
 */

#include <SDL/SDL.h>

#include <stdint.h>

#include "sdl.h"
#include "feature.h"
#include "feature_sdl.h"

extern int water_lvl;

static uint32_t uhtocolor(uint16_t v)
{
    uint32_t ret = v >> 8;
    if(v > water_lvl + 0x1000)
    {
        ret = ret | (ret << 16) | (ret << 8); 
    }
    else if(v > water_lvl + 0x0100)
    {
        ret = v & 0xff00; 
    } else if(v > water_lvl)
    {
        ret = (ret << 16) | (ret << 8);
    } else if(v > water_lvl - 0x0200)
    {
        ret = ret | (ret << 8);
    }
    return ret;
}

void feature_sdl_blit(Feature *f, SDL_Surface *s, irect2 src, irect2 dst)
{
    int i, j;

    for(j = 0; j < src[DY]; j++)
    {
        for(i = 0; i < src[DX]; i++)
        {
            sdl_setPixel(s, (dst[X] + i) % dst[DX], dst[Y] + j % dst[DY], uhtocolor(feature_get(f, i + src[X], j + src[Y])));
        }
    }
}
