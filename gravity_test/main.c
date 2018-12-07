/**
 * main.c
 * jumpy
 * December 03, 2018
 * Brandon Surmanski
 */

// C is weird and doesn't define a 'bool' type by default.
// C expects you to just use an integer that is 0 or not 0.
// Include this standard library to use boolean logic and
// true/false constant values.
#include <stdbool.h>

// SDL is a library we use for creating a
// window and handle key events.
// All functions that start with SDL_ are
// from this library.
#include <SDL2/SDL.h>

// define constants for screen width and height.
// There isn't anything special about these dimensions,
// I've just been using them for projects for a long time
// and I kinda like them.
// This resolution is 'QVGA'.
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Global variables.
// Typically the less globals the better,
// but we'll make an exception for these.
static bool running = true;

// Define a 'struct' to hold Player variables.
typedef struct Player {
  float x;
  float y;
  float vx;
  float vy;
  bool on_floor;
} Player;

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
    if (e.type == SDL_QUIT) running = false;
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
  // handle key input. Will modify the player.
  handle_input(player);

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
  if (player->x >= SCREEN_WIDTH) {
    player->x = SCREEN_WIDTH - 1;
    player->vx = -player->vx;
  }

  // Apply velocity each timestep.
  player->x += player->vx;
  player->y += player->vy;
}

void draw(SDL_Texture *texture, Player *player) {
  // This will be a pointer to the pixel data in the texture.
  int *pixels;

  // This will be the number of bytes in a row before wrapping
  // around. The pixel data is 2D, but is represented as a 1D
  // array, so we need to know how long each row is.
  int pitch;

  // Locking a texture is required by SDL2 if you want
  // to do software reading/writing to a texture.
  // This function will populate 'pixels' and 'pitch'.
  SDL_LockTexture(texture, NULL, (void**) &pixels, &pitch);
  pitch /= 4; // make pitch relative to int instead of byte.

  // wacky stuff to draw player trail.
  // Fade the whole screen over time.
  int i, j;
  for (i = 0; i < SCREEN_HEIGHT; i++) {
    for (j = 0; j < SCREEN_WIDTH; j++) {
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

  // After we're done with the texture, we need to 'unlock' it.
  // Required by SDL2.
  SDL_UnlockTexture(texture);
}

// main function. Everything starts here.
int main(int unused_argc, char **unused_argv) {

  // Setup the SDL library.
  SDL_Init(SDL_INIT_VIDEO);

  // Create an SDL screen. This initializes the window.
  SDL_Window *window = SDL_CreateWindow(
          "Jumpy",
          SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          SCREEN_WIDTH, SCREEN_HEIGHT,
          0);

  // Create a new SDL renderer. This object does rendering stuff, idk.
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  // Create a texture to draw to.
  // SDL2 doesn't let you draw directly to the screen (unlike SDL1),
  // So we need to create this for our weird software rendering.
  // Software rendering isn't super common these days, most of the
  // time things will use the GPU to draw, which is *much* faster.
  // But it is also more confusing, so lets just use software rendering
  // for now.
  SDL_Texture *texture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           SCREEN_WIDTH, SCREEN_HEIGHT);

  // create a new player.
  Player player = create_player();

  // track time between frames.
  int start_tick, end_tick, delta_tick;

  // main game loop.
  while (running) {
    // Start time of current frame.
    start_tick = SDL_GetTicks();

    // update game state. Will modify the player.
    // If I had more game state, I would consider making
    // a 'Game' struct to hold everything and pass that
    // into update and draw.
    update(&player);

    // draw the player.
    draw(texture, &player);

    // Render the texture.
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Draw what we've currently rendered to the screen.
    // It might seems like SDL_RenderCopy is doing the
    // same thing, but SDL requires this two step process
    // of drawing to the renderer, then to the screen.
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
