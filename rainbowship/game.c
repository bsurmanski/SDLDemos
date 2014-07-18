#include <SDL/SDL.h>

#include "render.h"
#include "ship.h"
#include "starfield.h"
#include "trigtable.h"


void update();
void draw();
int handle_events();
    
Ship* ship;
Starfield *stars;

int RUNNING = 1;

int run_game ()
{
    ship = get_ship(100,100);
    stars = get_starfield();
    
    while (RUNNING){
	    handle_events();
	    update();
	    draw();

	    SDL_Flip(SDL_GetVideoSurface());
	    SDL_Delay (16);
	}
	return 0;
}



void update()
{
    update_starfield(stars, get_ship_rotation(ship));
    update_ship(ship);
}	

void draw()
{
    clear_buffer(SDL_GetVideoSurface());
    draw_starfield(stars);
    draw_ship(ship);
}

int handle_events()
{
    SDL_Event event;
    int ret = 0;
 while( SDL_PollEvent( &event ) )
        {
            
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                RUNNING = 0;
                //Quit the program
              	ret = 1;
            }

            int mx = get_ship_vx(ship);
            int my = get_ship_vy(ship);
            int mr = get_ship_vr(ship);

            if (event.type == SDL_KEYDOWN){
                switch (event.key.keysym.sym){
                    case SDLK_LEFT:
                        mx = -1;
                        break;
                    case SDLK_RIGHT:
                        mx = 1;
                        break;
                    case SDLK_UP:
                        my = -1;
                        break;
                    case SDLK_DOWN:
                        my = 1;
                        break;
                    case SDLK_SPACE:
                        fire_bullet(ship);
                        break;
                    case SDLK_q:
                        //mr = 1;
                        break;
                    case SDLK_w:
                        //mr = -1;
                    default:
                        break;
                }
            }

            if (event.type == SDL_KEYUP){
                switch (event.key.keysym.sym){
                    case SDLK_LEFT:
                    case SDLK_RIGHT:
                        mx = 0;
                        break;
                    case SDLK_UP:
                    case SDLK_DOWN:
                        my = 0;
                        break;
                    case SDLK_q:
                    case SDLK_w:
                        mr = 0;
                    default:
                        break;
            }
        }
        set_ship_direction(ship, mx, my);
        set_ship_rotation_direction(ship, mr);
    }
	return ret;
}

