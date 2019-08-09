#include <SDL/SDL.h>
#include "game.h"

int screenx = 320, screeny = 240, screen_bit = 32;
SDL_Surface *buffer;

void init ()
{
	    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	    buffer = SDL_SetVideoMode( screenx, screeny, screen_bit, SDL_SWSURFACE );
        SDL_PixelFormat *fmt = buffer->format;
        printf("R %x\nG %x\nB %x\nA %x\n", fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);
        printf("%d\n", fmt->BytesPerPixel);
	    SDL_WM_SetCaption( "Microship", NULL );
}


int main (int argc, char *argv[])
{
	init();
    run_game();
    SDL_Quit();
	return 0;
}
