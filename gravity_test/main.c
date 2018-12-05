/**
 * main.c
 * jumpy
 * December 03, 2018
 * Brandon Surmanski
 */

#include <string.h>
#include <stdbool.h>

// SDL is a library we use for creating a
// window and handle key events.
// All functions that start with SDL_ are
// from this library.
#include <SDL2/SDL.h>

// Define a 'struct' to hold Player variables.
typedef struct Player {
  float x;
  float y;
  float vx;
  float vy;
  bool on_floor;
} Player;

// Global variables.
// Typically the less globals the better,
// but we'll make an exception for these.
static int running = 1;
static int paused = 1;

// Create and initialize a new player.
Player create_player() {
  Player player;
  player.x = 100;
  player.y = 100;
  player.vx = 1.0f;
  player.vy = -2.0f;
  player.on_floor = false;
  return player;
}

// Set the RGBA value of a pixel at x,y.
void setPixel(int *pixels, int pitch, int x, int y, int color) {
    pixels[x + y * pitch] = color;
}

// Get the RGBA value of a pixel at x,y.
int getPixel(int *pixels, int pitch, int x, int y) {
  return pixels[x + y * pitch];
}

void handle_input(Player *player) {
  // Handle closing the window.
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) running = 0;
  }

  // Get the current keyboard state.
  SDL_PumpEvents();
  const uint8_t *keystate = SDL_GetKeyboardState(0);

  // control
  if (keystate[SDL_SCANCODE_RIGHT]) {
    player->vx += 0.2;
  }
  if (keystate[SDL_SCANCODE_LEFT]) {
    player->vx -= 0.2;
  }

  // handle jump
  if (keystate[SDL_SCANCODE_SPACE] &&
      player->on_floor) {
    player->vy = -2;
  }
}

void update(Player *player) {
  // constant wall y-coordinate
  const int wally = 120;

  // if the player is below the floor.
  // (y gets bigger going down)
  if (player->y >= wally) {
    player->y = wally;
    player->vy *= -0.5;
    player->vx *= 0.97;
    player->on_floor = true;

  } else {
    // apply gravity by speeding up the player downward.
      player->on_floor = false;
    player->vy += 0.1;
  }

  // bounce off left wall.
  if (player->x <= 0) {
    player->x = 1;
    player->vx = -player->vx;
  }

  // bounce off right wall.
  if (player->x >= 320) {
    player->x = 319;
    player->vx = -player->vx;
  }

  // Apply velocity each timestep.
  player->x += player->vx;
  player->y += player->vy;
}

void draw(SDL_Texture *texture, Player *player) {
  int w = 320;
  int h = 240;

  int *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, (void**) &pixels, &pitch);
  pitch /= 4; // make pitch relative to int instead of byte.

  // wacky stuff to draw player trail.
  // Fade the whole screen over time.
  int i, j;
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      int p = getPixel(pixels, pitch, j, i);
      // set topmost bit to zero.
      p &= 0x7fffffff;
      // shift all bits in pixel to the right.
      // This has weird color effects because
      // it treats the RGBA bytes as a single (int)
      // value and pushes the bits between them.
      // Since the left most bit is zero, eventually
      // all the bits are push off the end (over
      // the course of ~32 frames) and the
      // pixel will be black.
      p >>= 1;
      setPixel(pixels, pitch, j, i, p);
    }
  }

  // Create a + shaped player, drawing each of the 5 pixels.
  setPixel(pixels, pitch, (int)player->x, (int)player->y, 0xFFFFFFFF);
  setPixel(pixels, pitch, (int)player->x + 1, (int)player->y, 0xFFFFFFFF);
  setPixel(pixels, pitch, (int)player->x - 1, (int)player->y, 0xFFFFFFFF);
  setPixel(pixels, pitch, (int)player->x, (int)player->y + 1, 0xFFFFFFFF);
  setPixel(pixels, pitch, (int)player->x, (int)player->y - 1, 0xFFFFFFFF);

  SDL_UnlockTexture(texture);
}

// main function. Everything starts here.
int main(int unused_argc, char **unused_argv) {
  // Setup the SDL library.
  SDL_Init(SDL_INIT_VIDEO);

  // create an SDL screen. This initializes the window.
  SDL_Window *window = SDL_CreateWindow(
          "Jumpy",
          SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          320, 240,
          0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Texture *texture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           320, 240);

  // create a new player.
  Player player = create_player();

  // track time between frames.
  int start_tick, end_tick, delta_tick;

  // main game loop.
  while (running) {
    // Start time of current frame.
    start_tick = SDL_GetTicks();
    // update everything.
    handle_input(&player);
    update(&player);
    draw(texture, &player);

    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Draw 'renderer' context to the screen.
    SDL_RenderPresent(renderer);

    // Delay a bit so we don't go a million frame per second.
    end_tick = SDL_GetTicks();
    delta_tick = end_tick - start_tick;
    if (delta_tick < 16) {
      // Wait up to 16ms per frame. Aiming for 60fps.
      SDL_Delay(16 - delta_tick);
    }
  }

  SDL_DestroyWindow(window);

  return 0;
}
