/**
 * world.c
 * earth
 * November 10, 2012
 * Brandon Surmanski
 */

#include <pthread.h>

#include <stdlib.h>

#include "feature.h"
#include "feature_terrain.h"

#include "world.h"

static void *world_init_thread(void *args)
{
    World *world = (World*) args;
    feature_terrain_create(world->height);
    return NULL;
}

void world_init(World *world, int w, int h)
{
    world->w = w;
    world->h = w;
    world->height = malloc(sizeof(Feature));
    world->lithosphere = malloc(sizeof(Feature));
    feature_init(world->height, w, h);
    pthread_t thread;
    pthread_create(&thread, NULL, world_init_thread, world); 
}

void world_finalize(World *w)
{
    free(w->height);
    free(w->lithosphere);
}
