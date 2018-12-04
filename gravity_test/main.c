/**
 * main.c
 * jumpy
 * December 03, 2018
 * Brandon Surmanski
 */

#include <string.h>

// SDL is a library we use for creating a
// window and handle key events.
#include <SDL/SDL.h>

// Define a 'struct' to hold Player variables.
typedef struct Player {
    float x;
    float y;
    float vx;
    float vy;
} Player;

// Global variables.
// Typically the less globals the better,
// but we'll make an exception for these.
static int running = 1;
static int paused = 1;
static SDL_Surface *screen;

// Create and initialize a new player.
Player create_player() {
    Player player;
    player.x = 100;
    player.y = 100;
    player.vx = 1.0f;
    player.vy = -2.0f;
    return player;
}

// check if a given x,y is a valid surface coordinate.
int inSurface(SDL_Surface *s, int x, int y)
{
    return (x > 0 && x < s->w && y > 0 && y < s->h);
}

// Set the RGBA value of a pixel at x,y.
void setPixel(SDL_Surface *s, int x, int y, int color)
{
    if(inSurface(s,x,y)){
        memcpy(
                s->pixels + x * s->format->BytesPerPixel + y * s->pitch,
                &color,
                s->format->BytesPerPixel
                );
    }
}

// Get the RGBA value of a pixel at x,y.
int getPixel(SDL_Surface *s, int x, int y)
{
    int ret = 0;
    if(inSurface(s,x,y)){
        memcpy(&ret,
                s->pixels + x * s->format->BytesPerPixel + y * s->pitch,
                s->format->BytesPerPixel);
    }
    return ret;
}

void update(Player *player)
{
    // Handle closing the window.
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT)
            running = 0;
    }

    // Get the current keyboard state.
    SDL_PumpEvents();
    uint8_t *keystate = SDL_GetKeyState(0);

    // control
    if(keystate[SDLK_RIGHT]) {
        player->vx += 0.2;
    }
    if(keystate[SDLK_LEFT]) {
        player->vx -= 0.2;
    }

    // constant wall y-coordinate
    const int wally = 120;

    // if the player is below the floor.
    // (y gets bigger going down)
    if (player->y >= wally) {
        player->y = wally;
        player->vy *= -0.5;
        player->vx *= 0.97;

        // handle jump
        if(keystate[SDLK_SPACE]) {
            player->vy = -2;
        }
    } else {
        // apply gravity by speeding up the player.
        player->vy += 0.1;
    }

    // bounce off left wall.
    if (player->x < 0) {
        player->x = 1;
        player->vx = -player->vx;
    }

    // bounce off right wall.
    if (player->x > 320) {
        player->x = 319;
        player->vx = -player->vx;
    }

    // Apply velocity each timestep.
    player->x += player->vx;
    player->y += player->vy;

    // wacky stuff to draw player trail.
    // Fade the whole screen over time.
    int i, j;
    for(i = 0; i < screen->h; i++) {
        for(j = 0; j < screen->w; j++) {
            int p = getPixel(screen, j, i);
            p &= 0x7fffffff;
            p >>= 1;
            setPixel(screen, j, i, p);
        }
    }

    // Create a + shaped player, drawing each of the 5 pixels.
    setPixel(screen, (int) player->x, (int) player->y, 0xFFFFFFFF);
    setPixel(screen, (int) player->x+1, (int) player->y, 0xFFFFFFFF);
    setPixel(screen, (int) player->x-1, (int) player->y, 0xFFFFFFFF);
    setPixel(screen, (int) player->x, (int) player->y+1, 0xFFFFFFFF);
    setPixel(screen, (int) player->x, (int) player->y-1, 0xFFFFFFFF);
}

// main function. Everything starts here.
int main (int unused_argc, char **unused_argv)
{
    // create an SDL screen. This initializes the window.
    screen = SDL_SetVideoMode(320, 240, 0, SDL_SWSURFACE);

    // set a title for the window.
    SDL_WM_SetCaption("Jumpy", 0);

    // clear all of the pixels on the screen.
    // (set the memory to all 0's. (black))
    memset(screen->pixels, 0, screen->pitch * screen->h);

    // create a new player.
    Player player = create_player();

    // track time between frames.
    int start_tick, end_tick, delta_tick;

    // main game loop.
    while(running)
    {
        // Start time of current frame.
        start_tick = SDL_GetTicks();
        // update everything.
        update(&player);

        // Delay a bit so we don't go a million frame per second.
        end_tick = SDL_GetTicks();
        delta_tick = end_tick - start_tick;
        if(delta_tick < 16){
            // Wait up to 16ms per frame. Aiming for 60fps.
            SDL_Delay(16 - delta_tick);
        }

        // screen is 'double buffered'.
        // We need to call this so drawing works.
        SDL_Flip(screen);
    }

    return 0;
}
