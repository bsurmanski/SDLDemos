/**
 * main.c
 * glModern
 * Testing modern OpenGL syntax and methods
 * December 5, 2011
 * Brandon Surmanski
 */

#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>

#include "map.h"

void draw_ts(void)
{
    int i;
    int j;
    int col;
    map_initDiamondSquare(256,256);
    for(j = 0; j < map_getHeight(); j++){
        for(i = 0; i < map_getWidth(); i++){
            col = (int) 255 * map_getData()[i + j * map_getWidth()];
            col = SDL_MapRGB(SDL_GetVideoSurface()->format, col, col, col);
            memcpy(SDL_GetVideoSurface()->pixels + (i + SDL_GetVideoSurface()->w * j) * SDL_GetVideoSurface()->format->BytesPerPixel, &col, SDL_GetVideoSurface()->format->BytesPerPixel);
        }
    }
}

int handleEvents(void)
{
    SDL_PumpEvents();
    uint8_t *keyState = SDL_GetKeyState(0);
    return !keyState[SDLK_SPACE];
}

void run(void)
{
    int running = 1;
    draw_ts();
    while (running){
        running = handleEvents();
        SDL_Flip(SDL_GetVideoSurface());
        SDL_Delay(16);
    }
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetVideoMode(640, 480, 0, SDL_SWSURFACE);
    srand(time(0));
    run();
    return 0;
}
