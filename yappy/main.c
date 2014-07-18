/**
 * main.c
 * yappy
 * October 23, 2011
 * Brandon Surmanski
 */

#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "yappy.h"

SDL_Surface *main_screen;

int main(int argc, char **argv)
{
    int err;
    err = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if (err) {printf("cannot init sdl"); return -1;}

    main_screen = SDL_SetVideoMode(320, 240, 8, SDL_HWSURFACE);
    if (!main_screen){printf("cannot init sdl video"); return -1;}

    err = Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 4096);
    if (err) {printf("unable to initialize audio: %s\n", Mix_GetError());}

    yappy_load();
    yappy_loop();

    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}
