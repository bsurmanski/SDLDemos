/**
 * main.c
 * earth
 * November 6, 2012
 * Brandon Surmanski
 */

#include <SDL/SDL.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

#include "clockwork/util/math/rect.h"
#include "clockwork/util/math/scalar.h"
#include "clockwork/util/noise.h"

#include "world.h"
#include "feature_sdl.h"
#include "sdl.h"

#define INDEX(arr, w, h, x, y) (((char*)arr)+x+w*y)

static SDL_Surface *s;
bool running = false;
int WIDTH = 640;
int HEIGHT = 480;
int MAP_WIDTH = 640;
int MAP_HEIGHT = 480;

int XPOS = 0;
int YPOS = 0;

static World world;
int water_lvl = 0x8200;


static void init(void)
{
    world_init(&world, MAP_WIDTH, MAP_HEIGHT);
}

static void events(void)
{
    SDL_PumpEvents();
    uint8_t *key_state = SDL_GetKeyState(0);
    if(key_state[SDLK_SPACE])
    {
        running = false;
    }

    if(key_state[SDLK_LEFT])
    {
        XPOS-=5;
    }
    if(key_state[SDLK_RIGHT])
    {
        XPOS+=5; 
    }

    if(key_state[SDLK_UP])
    {
        YPOS-=5;
    }
    if(key_state[SDLK_DOWN])
    {
        YPOS+=5; 
    }
    if(key_state[SDLK_a])
    {
        water_lvl-=5;
    }
    if(key_state[SDLK_s])
    {
        water_lvl+=5; 
    }
}

static void draw(void)
{
    irect2 srcrect = {XPOS, YPOS, MAP_WIDTH, MAP_HEIGHT};
    irect2 dstrect = {0, 0, WIDTH, HEIGHT};
    feature_sdl_blit(world.height, s, srcrect, dstrect);
}

static void run(void)
{
    running = true;
    init();
    while(running)
    {
        events();
        draw();
        SDL_Flip(s);
        SDL_Delay(16);
    }
}

int main(int argc, char **argv)
{
    s = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_SWSURFACE);
    srand(time(0));
    noise_init(time(0));
    run();
    return 0;
}
