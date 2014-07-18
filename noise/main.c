/**
 * main.c
 * noise
 * November 11, 2011
 * Brandon Surmanski
 */

#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "vec.h"
#include "noise.h"

static int quit = 0;
static int count = 0;

void apply_perlin(SDL_Surface *s){
    int i;
    int j;
    void *pxl;
    float perlin;
    uint32_t color;
        for(j = 0; j < s->w; j++){
             perlin = noise_fracPerlin1((j+count)/100.0f,1)/3;
             perlin = perlin - ((int)perlin);
             perlin += 0.5;
             perlin *= 255;
             color = SDL_MapRGB(s->format, (int) 255, (int)255, (int)255);
             for(i = (int) perlin*2/3; i < s->h; i++){
                 pxl = s->pixels + i * s->pitch + j * s->format->BytesPerPixel;
                 memcpy(pxl, (void*)&color, s->format->BytesPerPixel);
             }
        }
}

void apply_perlin2(SDL_Surface *s)
{
    init();
    int i,j;
    void *pxl;
    float perlin;
    uint32_t color;
    noise_seed(time(0));
    for(i = 0; i < s->h; i++)
        for(j = 0; j < s->w; j++){
                 float vec[2] = {j/80.0f, i/80.0f};
                 perlin = noise_terbulence2(vec[0], vec[1], 1); 
                 //perlin = noise_perlin1(vec[0]);
                 //printf("%f\n", perlin);
                 perlin += 1.0f;
                 perlin /= 2.0f;
                 perlin *= 255;
                 color = SDL_MapRGB(s->format, (int) perlin, (int)perlin, (int)perlin);
                 pxl = s->pixels + i * s->pitch + j * s->format->BytesPerPixel;
                 memcpy(pxl, (void*)&color, s->format->BytesPerPixel);
        }
}

void handlekeys(void)
{
    SDL_PumpEvents();
    uint8_t *keystate = SDL_GetKeyState(0);
    if(keystate[SDLK_SPACE]){
        quit = 1;
    }
}

int main(int argc, char **argv)
{
    SDL_Surface *s = SDL_SetVideoMode(320, 240, 0, SDL_SWSURFACE);
        init();
        apply_perlin2(s);
    while(!quit){
        int timer = SDL_GetTicks();
        count++;
        //SDL_FillRect(s, 0, 0);
        //apply_perlin(s);
        handlekeys();
        SDL_Flip(s);
        timer = SDL_GetTicks() - timer;
        if(timer < 16)
            SDL_Delay(16 - timer);
    }
    return 0;
}
