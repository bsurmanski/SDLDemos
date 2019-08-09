/**
 * main.c
 * yappy
 * October 23, 2011
 * Brandon Surmanski
 */

#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image/SDL_image.h>
#include <SDL_mixer/SDL_mixer.h>

#include "yappy.h"

SDL_Surface *main_screen;

int main(int argc, char **argv)
{
    int err;
    err = SDL_Init(SDL_INIT_EVERYTHING);
    if (err) {printf("cannot init sdl"); return -1;}

    main_screen = SDL_SetVideoMode(320, 240, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!main_screen){printf("cannot init sdl video"); return -1;}

    err = Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 4096);
    if (err) {printf("unable to initialize audio: %s\n", Mix_GetError());}

    if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG)) {
        printf("issues initting image\n");
    }

    yappy_load();
    yappy_loop();

    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}
