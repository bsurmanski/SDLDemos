/**
 * main.c
 * life
 * November 16, 2011
 * Brandon Surmanski
 */

#include <sys/time.h>
#include <time.h>
#include <string.h>

#include <SDL/SDL.h>

static int running = 1;
static int paused = 1;
static SDL_Surface *screen;
static SDL_Surface *backbuffer;

void *pixelAt(SDL_Surface *s, int x, int y)
{
    return (s->pixels + x * s->format->BytesPerPixel + y * s->pitch);
}

int pixelIsWhite(SDL_Surface *s, int x, int y)
{
    int white = 0xFFFFFFFF;
    if(x<0)
        x+=s->w;
    if(y<0)
        y+=s->h;
    if(x>=s->w)
        s-=s->w;
    if(y>=s->h)
        s-=s->h;
    return !memcmp(pixelAt(s,x,y), &white, s->format->BytesPerPixel);
}

void setPixel(SDL_Surface *s, int x, int y, int val)
{
    memcpy(pixelAt(s, x, y), &val, s->format->BytesPerPixel);
}

void copyBuffer(SDL_Surface *dest, SDL_Surface *src)
{
    memcpy(dest->pixels, src->pixels, dest->pitch * dest->h);
}

int selx = 0, sely = 0;
static void handlekeys()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT)
            running = 0;
        if(e.type == SDL_MOUSEMOTION){
            selx = e.button.x;
            sely = e.button.y;
        }
        if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_z)
                paused = paused ? 0 : 1;
        }
    }
    SDL_PumpEvents();
    uint8_t *keystate = SDL_GetKeyState(0);
    if(keystate[SDLK_RETURN])
        running = 0;
    if(keystate[SDLK_RIGHT])
        selx++;
    if(keystate[SDLK_LEFT])
        selx--;
    if(keystate[SDLK_UP])
        sely--;
    if(keystate[SDLK_DOWN])
        sely++;
    if(selx < 0)
        selx = 0;
    if(sely < 0)
        sely = 0;
    if(selx > screen->w)
        selx = screen->w;
    if(sely > screen->h)
        sely = screen->h;

    if(keystate[SDLK_SPACE]){
        setPixel(backbuffer, selx, sely, 0xFFFFFFFF);
    }

    if(SDL_GetMouseState(0, 0))
        setPixel(backbuffer, selx, sely, 0xFFFFFFFF);

    if(keystate[SDLK_z]){
    }
}

void update()
{
    if(!paused){
    int i, j, neighbors;
        for(i = 0; i < (screen->h-1); i++){
            for(j = 0; j < (screen->w-1); j++){
                neighbors = 0;
                if(pixelIsWhite(screen, j, i-1))
                    neighbors++;
                if(pixelIsWhite(screen, j, i+1))
                    neighbors++;
                if(pixelIsWhite(screen, j-1, i))
                    neighbors++;
                if(pixelIsWhite(screen, j+1, i))
                    neighbors++;

                if(pixelIsWhite(screen, j-1, i-1))
                    neighbors++;
                if(pixelIsWhite(screen, j-1, i+1))
                    neighbors++;
                if(pixelIsWhite(screen, j+1, i+1))
                    neighbors++;
                if(pixelIsWhite(screen, j+1, i-1))
                    neighbors++;

                if(neighbors < 2)
                    setPixel(backbuffer, j, i, 0x00000000);
                if(neighbors == 3)
                    setPixel(backbuffer, j, i, 0xFFFFFFFF);
                if(neighbors > 3)
                    setPixel(backbuffer, j, i, 0x00000000);
            }
        }
    }
    copyBuffer(screen, backbuffer);
}

void initBufferToRandom(SDL_Surface *buffer)
{
    srand(time(0));
    int i, j;
    for (i = 0; i < buffer->h; i++)
        for(j=0; j< buffer->w; j++){
            if (rand() % 10 < 1)
                setPixel(buffer, j, i, 0xFFFFFFFF);
        }
}

int main (int argc, char **argv)
{
    screen = SDL_SetVideoMode(320, 240, 0, SDL_SWSURFACE);
    SDL_WM_SetCaption("Life", 0);
    backbuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
    memset(screen->pixels, 0, screen->pitch * screen->h);
    initBufferToRandom(backbuffer);
    while(running)
    {
        handlekeys();
        update();
        SDL_Flip(screen);
        SDL_Delay(32);
    }
}
