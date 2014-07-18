/**
 * main.c
 * veronoi
 * November 22, 2011
 * Brandon Surmanski
 */

#include <SDL/SDL.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>

#include "time.h"
#include "clockwork/util/math/vec.h"
#include "clockwork/util/struct/iterator.h"
#include "clockwork/util/struct/kdtree.h"
#include "clockwork/util/noise.h"

static kdtree tree;
static SDL_Surface *s;
static int running = 1;
static float SCALE = 5.0f;

static int N_CELLS = 100;
static int N_PLATES = 15;

static int WIDTH = 640;
static int HEIGHT = 480;

static void setpixel(SDL_Surface *s, int i, int j, int val)
{
    int bpp = s->format->BytesPerPixel;
    void *pxl = s->pixels + j * s->pitch + i * bpp; 
    memcpy(pxl, &val, bpp);
}

static int getpixel(SDL_Surface *s, int i, int j)
{
    int val;
    int bpp = 4;
    void *pxl = s->pixels + j * s->pitch + i * bpp;
    memcpy((void*)&val, pxl, bpp);
    return val;
}

static int packid(int id)
{
    return id * 100;
}

static int unpackid(int id)
{
    return id & 0xff;
}

float clamp(float x, float min, float max)
{
    return (x < min ? min : (x > max ? max : x));
}

float dist(float a[], float b[], int dim)
{
    float sqsum = 0.0f;
    int i;
    for(i = 0; i < dim; i++)
    {
        sqsum += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sqrt(sqsum);
}

void init_veronoi(SDL_Surface *s)
{
    kdtree_init(&tree, 2);

    int i;
    float pos[2];
    int *id = malloc(sizeof(int) * N_PLATES);
    for(i = 0; i < N_CELLS; i++){
        pos[0] = WIDTH * (abs(rand()) / (float) RAND_MAX);
        pos[1] = HEIGHT * (abs(rand()) / (float) RAND_MAX);
        if(i < N_PLATES)
        {
            id[i] = (i + 1);
            kdtree_insert(&tree, pos, &id[i]);
        } else 
        {
            int *closeid = kdtree_closest(&tree, pos, NULL);
            kdtree_insert(&tree, pos, closeid);
        }
    }
}

void apply_veronoi(SDL_Surface *s)
{
    int i,j;
    for(j = 0; j < HEIGHT; j++)
        for(i = 0; i < WIDTH; i++){
            float pos[2] = {i, j};
            float *closest[10];
            void *closestval[10];
            int n = kdtree_closestN(&tree, pos, 5, closestval, closest);
            //float f = (dist(closest[2], pos, 2) - dist(closest[0], pos, 2)) * SCALE;
            float f = (dist(closest[1], pos, 2) - dist(closest[0], pos, 2)) * SCALE;
            f = clamp(f, 0, 250);
            uint32_t color = SDL_MapRGB(s->format, (int) f, (int) f, (int) f);
            //int color = //packid(*((int*)closestval[0]));
            void *pxl = s->pixels + j * s->pitch + i * s->format->BytesPerPixel;
            memcpy(pxl, (void*)&color, s->format->BytesPerPixel);
        }
}

void apply_veronoi2sub1(SDL_Surface *s)
{
    int i,j;
    for(j = 0; j < HEIGHT; j++)
        for(i = 0; i < WIDTH; i++){
            float pos[2] = {i, j};
            float *closest[10];
            void *closestval[10];
            int n = kdtree_closestN(&tree, pos, 10, closestval, closest);
            float f;

            int k=1;
            for(k = 1; k < 8; k++)
            {
                if(closestval[k] != closestval[0])
                {
                    break;
                }
            }
            f = (dist(closest[k], pos, 2) - dist(closest[0], pos, 2)) * SCALE;
            f = clamp(f, 0, 250);
            uint32_t color = SDL_MapRGB(s->format, (int) f, (int) f, (int) f);
            void *pxl = s->pixels + j * s->pitch + i * s->format->BytesPerPixel;
            memcpy(pxl, (void*)&color, s->format->BytesPerPixel);
        }
}

void apply_veronoi_perlin(SDL_Surface *s)
{
    int i,j;
    for(j = 0; j < HEIGHT; j++)
        for(i = 0; i < WIDTH; i++){
            float pos[2] = {i, j};
            float *closest[10];
            void *closestval[10];
            int n = kdtree_closestN(&tree, pos, 10, closestval, closest);
            float f = 0.0f;

            if(*((int*)closestval[0]) > 150)
            {
                int k;
                for(k = 1; k < 8; k++)
                {
                    if(closestval[k] != closestval[0])
                    {
                        break;
                    }
                }
                f = (dist(closest[k], pos, 2) - dist(closest[0], pos, 2)) * SCALE;
                f = clamp(f, 0, 250);
                f /= 250.0f;
                f *= (noise2_fracPerlin(i / 50.0f, j / 50.0f, 4) + 1) / 2.0f;
                f *= SCALE;
                f *= 50.0f;
                f = clamp(f, 0, 250);
            }
            uint32_t color = SDL_MapRGB(s->format, (int) f, (int) f, (int) f);
            void *pxl = s->pixels + j * s->pitch + i * s->format->BytesPerPixel;
            memcpy(pxl, (void*)&color, s->format->BytesPerPixel);
        }
}

/**
 * finds 2 random cells, and assigns makes them have the same ID
 */
void merge_random_cells(void)
{
    float pos[2];
    pos[0] = WIDTH * (abs(rand()) / (float) RAND_MAX);
    pos[1] = HEIGHT * (abs(rand()) / (float) RAND_MAX);

    float *closest[3];
    void *closestval[3];
    int n = kdtree_closestN(&tree, pos, 3, closestval, closest);
    memcpy(closestval[1], closestval[0], sizeof(int));
}

static void *techtonics_start(void *args)
{
    SDL_Surface *back = args; 

    vec2 *velocity = alloca(sizeof(vec2) * N_PLATES);

    int i,j;
    for(i = 0; i < N_PLATES; i++)
    {
        vec2_rand(velocity[i]);
    }

    for(j = 0; j < HEIGHT; j++)
    {
        for(i = 0; i < WIDTH; i++)
        {
            int val = unpackid(getpixel(s, i, j));
            int dstx = iwrap(i + velocity[val][X] * 5, WIDTH - 1);
            int dsty = iwrap(j + velocity[val][Y] * 5, HEIGHT - 1);
            setpixel(back, dstx, dsty, val); 
        }
    }
}

void run_techtonics(SDL_Surface *s)
{
    pthread_t thread;
    //pthread_create(&thread, NULL, techtonics_start, s);
    techtonics_start(s);
}

void handle_events(void)
{
    SDL_PumpEvents();
    uint8_t *key_state;
    key_state = SDL_GetKeyState(0);
    if(key_state[SDLK_SPACE])
        running = 0;

    if(key_state[SDLK_a])
    {
        struct timeval t;
        timeval_tick(&t);

        float pos[2];
        pos[0] = WIDTH * (abs(rand()) / (float) RAND_MAX);
        pos[1] = HEIGHT * (abs(rand()) / (float) RAND_MAX);
        kdtree_removeClosest(&tree, pos, NULL);

        printf("remove: %f\n", timeval_tick(&t));

        apply_veronoi(s);

        printf("apply: %f\n\n", timeval_tick(&t));
    }

    if(key_state[SDLK_s])
    {
        int i;
        for(i = 0; i < N_CELLS; i++)
        {
            merge_random_cells();
        }
        apply_veronoi(s);
    }

    if(key_state[SDLK_d])
    {
        apply_veronoi2sub1(s);
    }

    if(key_state[SDLK_x])
    {
        apply_veronoi_perlin(s);
    }

    if(key_state[SDLK_t])
    {
        /*
        SDL_Surface *back = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, 32, 
                0x000000ff,
                0x0000ff00,
                0x00ff0000,
                0xff000000);*/
        SDL_Surface *back = SDL_ConvertSurface(s, s->format, s->flags);
        SDL_FillRect(back, NULL, 0);
        SDL_LockSurface(back);
        run_techtonics(back);
        SDL_UnlockSurface(back);
        SDL_FreeSurface(back);
    }
}

int main(int argc, char **argv)
{
    srand(time(0));
    noise_init(time(0));
    SDL_Init(SDL_INIT_VIDEO);
    s = SDL_SetVideoMode(WIDTH, HEIGHT, 0, SDL_SWSURFACE);
    init_veronoi(s);
    apply_veronoi(s);
    while (running){
        handle_events();
        SDL_Flip(s);
    }
    return 0;
}
