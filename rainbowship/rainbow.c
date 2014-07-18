#include "rainbow.h"
#include "render.h"
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

void add_rainbow_line(SDL_Surface *fb, int x, int y)
{
    int pxl = 0xff0000; // red!
    draw_pixel(x-3, y, fb, &pxl);
    pxl = 0xff7f00; //orange!
    draw_pixel(x-2, y, fb, &pxl);
    pxl = 0xffff00; //yellow!
    draw_pixel(x-1, y, fb, &pxl);
    pxl = 0x00ff00; //green!
    draw_pixel(x, y, fb, &pxl);
    pxl = 0x007fff; //blue!
    draw_pixel(x+1, y, fb, &pxl);
    pxl = 0x0000ff; //indigo!
    draw_pixel(x+2, y, fb, &pxl);
    pxl = 0x7f007f; //violet!
    draw_pixel(x+3, y, fb, &pxl);
    //yes, if could have been more concise if it were in a loop. But the color picking is a bit wacky, so sequence it is!
}

void draw_rainbow(SDL_Surface *fb, int x, int y, int vx, int vy, int num_pixels)
{
    int i;
    for (i = 0; i < num_pixels-vy; i++){
        add_rainbow_line(fb, x, y + i);
    }
}

void update_rainbow(SDL_Surface *fb, int x, int y, int vx, int vy, int num_pixels)
{
    memmove(fb->pixels + fb->pitch*num_pixels, fb->pixels, fb->pitch*(fb->h-num_pixels));
}



