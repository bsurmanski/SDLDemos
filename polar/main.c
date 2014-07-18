/**
 * main.c
 * polar
 * November 24, 2011
 * Brandon Surmanski
 */

#include <math.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "noise.h"

#define PI 3.141592654

static int running = 1;
static float r = 10;
static float rot = 0;
static SDL_Surface *s;

static void setPixel(SDL_Surface *s, int x, int y, int val)
{
    memcpy(s->pixels + x * s->format->BytesPerPixel + y * s->pitch, &val, s->format->BytesPerPixel);
}

static void handleEvents(void)
{
    SDL_PumpEvents();
    uint8_t *key_state = SDL_GetKeyState(0);
    if(key_state[SDLK_SPACE])
        running = 0;

    if(key_state[SDLK_UP])
        r+=0.05*r;

    if(key_state[SDLK_DOWN])
        r-=0.05*r;

    if(key_state[SDLK_LEFT])
        rot+=0.1;

    if(key_state[SDLK_RIGHT])
        rot-=0.1;

    if(r<=0)
        r=0.1;

    if(rot > 2 * PI)
        rot-=2*PI;
    if(rot < 0)
        rot+= 2*PI;
}

static void draw(void)
{
    float dc = 0.5f/(r);
    float i;
    int x,y;
    float start, end;
    start = 0; end = 2 * PI;
    for(i = start; i < end; i+= dc)
    {
        float n0 = noise_fracPerlin1(i, 5);
        float n1 = noise_fracPerlin1(i-(2*PI),5);
        float dr = (((2*PI)-i) * n0 + i * n1)/(2*PI);
        dr+=1;
        dr/=2;
        dr *= r;
        dr+=r;
        x = (int)(dr * cos(i) + s->w/2);
        y = (int)(dr * sin(i) + s->h/2 + r*1.5);
        //printf("x:%i, y:%i, i:%f\n",x,y,i);
        if(x>0 && y>0 && x < s->w && y < s->h)
            setPixel(s, x, y, 0xFFFFFFFF);
    }
}

static void update(void)
{

}

int main(int argc, char **argv)
{
    s = SDL_SetVideoMode(320, 240, 0, SDL_SWSURFACE);
    while(running){
        handleEvents();
        update();
        SDL_FillRect(s,0,0);
        draw();
        SDL_Flip(s);
        SDL_Delay(16);
    }
    return 0;
}
