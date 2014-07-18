/**
 * world.h
 * earth
 * November 10, 2012
 * Brandon Surmanski
 */

#ifndef _WORLD_H
#define _WORLD_H

struct Feature;

typedef struct World
{
    int w;
    int h;
    struct Feature *height;
    struct Feature *lithosphere;
} World;

void world_init(World *world, int w, int h);
void world_finalize(World *w);

#endif
