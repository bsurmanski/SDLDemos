/**
 * main.c
 * fire
 * November 21, 2011
 * Brandon Surmanski
 */
#include <SDL/SDL.h>
#include <stdlib.h>

static int running = 1;

void handle_events(void)
{
    SDL_PumpEvents();
    uint8_t *key_state = SDL_GetKeyState(0);
    if(key_state[SDLK_SPACE])
        running = 0;
}

void *getPixelAt(SDL_Surface *s, int x, int y)
{
    return s->pixels + x * s->format->BytesPerPixel + y * s->pitch;
}

void setPixel(SDL_Surface *s, int x, int y, int val)
{
    memcpy(getPixelAt(s,x,y), &val, s->format->BytesPerPixel);
}

void fire(SDL_Surface *s)
{
    int i,j;
    void *p;
    //clear bottom pixels
    memset(s->pixels + s->pitch * (s->h-1), 0, s->pitch);
    //shift entire frame up
    //memcpy(s->pixels, s->pixels + s->pitch, s->pitch * (s->h-1));
    //randomly light bottom
    for(i = 0; i < s->w; i++){
        if((rand() % 100) < 80){
            setPixel(s, i, s->h-2, SDL_MapRGB(s->format, 200, rand() % 200, 0));
        }else {
            setPixel(s, i, s->h-2, SDL_MapRGB(s->format, 0, 0, 0));
        }
    }
    //average values
    int rs, gs, bs;
    uint8_t r0, g0, b0;
    for(i = 1; i < s->h-2; i++)
        for(j = 1; j < s->w-1; j++)
        {
           // p = getPixelAt(s,j,i);
           // SDL_GetRGB(*(uint32_t*)p, s->format, &r0, &g0, &b0);
            //rs = r0; gs = g0; bs = b0;
            rs = 0; gs = 0; bs = 0;

            p = getPixelAt(s,j+1,i+1);
            SDL_GetRGB(*(uint32_t*)p, s->format, &r0, &g0, &b0);
            rs += r0; gs += g0; bs += b0;

            p = getPixelAt(s,j-1,i+1);
            SDL_GetRGB(*(uint32_t*)p, s->format, &r0, &g0, &b0);
            rs += r0; gs += g0; bs += b0;

            p = getPixelAt(s,j,i+2);
            SDL_GetRGB(*(uint32_t*)p, s->format, &r0, &g0, &b0);
            rs += r0; gs += g0; bs += b0;

            p = getPixelAt(s,j,i-0);
            SDL_GetRGB(*(uint32_t*)p, s->format, &r0, &g0, &b0);
            rs += r0-1; gs += g0-1; bs += b0-1;

            rs/=4; gs/=4; bs/=4;

            setPixel(s, j, i, SDL_MapRGB(s->format, rs, gs, bs));
        }
}

int main(int argc, char **argv)
{
    SDL_Surface *s = SDL_SetVideoMode(320, 240, 32, SDL_SWSURFACE);
    srand(time(0));
    while(running){
        handle_events();
        fire(s);
        SDL_Flip(s);
        SDL_Delay(16);
    }
}
