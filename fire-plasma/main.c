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
    void *p1,*p2,*p3,*p4,*p0;
    //clear bottom pixels
    memset(s->pixels + s->pitch * (s->h-1), 0, s->pitch);
    //shift entire frame up
    memcpy(s->pixels, s->pixels + s->pitch, s->pitch * (s->h-1));
    //randomly light bottom
    for(i = 0; i < s->w; i++){
        if((rand() % 100) < 60){
            setPixel(s, i, s->h-1, SDL_MapRGB(s->format, 255, 0, 0));
        }
    }
    //average values
    for(i = 1; i < s->h-1; i++)
        for(j = 1; j < s->w-1; j++)
        {
            p0 = getPixelAt(s,j,i);
            p1 = getPixelAt(s,j+1,i);
            p2 = getPixelAt(s,j-1,i);
            p3 = getPixelAt(s,j,i+1);
            p4 = getPixelAt(s,j,i-1);
            *(int*)p0 += *(int*)p1 + *(int*)p2 + *(int*)p3 + *(int*)p4;
            *(int*)p0/=5;
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
