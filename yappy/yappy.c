/**
 * yappy.c
 * yappy
 * October 23, 2011
 * Brandon Surmanski
 */

#include <stddef.h>
#include <stdbool.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include "yappy.h"

extern SDL_Surface *main_screen;

static SDL_Surface *yappy_closed;
static SDL_Surface *yappy_opened;
static Mix_Chunk *sqeek;

bool yappy_open;
static running;

void yappy_load(void)
{
    yappy_closed = IMG_Load("res/yappy-closed.png");
    yappy_opened = IMG_Load("res/yappy-open.png");
    sqeek = Mix_LoadWAV("onetone.wav");
    running = 1;
    if (!sqeek) printf("sound load error: %s\n", Mix_GetError());
    //if (!yappy_closed || !yappy_opened || !sqeek) running = 0;
}

static void handle_keys(void)
{
    SDL_PumpEvents();
    uint8_t *key_state = SDL_GetKeyState(NULL);
    yappy_open = key_state[SDLK_SPACE];
    running = !key_state[SDLK_RETURN];
}

static void draw(void)
{
    SDL_Rect fill = {0, 0, main_screen->w, main_screen->h};
    SDL_FillRect(main_screen, &fill, SDL_MapRGB(main_screen->format,
                255,255,255));
    SDL_Surface *current = yappy_open ? yappy_opened : yappy_closed;
    SDL_Rect yapper = { (int)(main_screen->w/2 - current->w/2),
                        (int)(main_screen->h/2 - current->h/2),
                        current->w,
                        current->h };
    SDL_BlitSurface(current, NULL, main_screen, &yapper);
}

static void sound(void)
{
    if (yappy_open){
        int err = Mix_PlayChannel(-1, sqeek, 0);
        if (err == -1) printf("cant play sound: %s\n", Mix_GetError());
    }
}

void yappy_loop(void)
{
   while(running){
       handle_keys();
       draw();
       sound();
       SDL_Flip(main_screen);
       SDL_Delay(16);
   }
}
