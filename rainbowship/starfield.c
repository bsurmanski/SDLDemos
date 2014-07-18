//custom libs
#include "vl_array.h"
#include "matrix.h"

//non-std libraries
#include <SDL/SDL.h>

//c std libraries
#include <stdlib.h>
#include <time.h>

typedef struct Starfield {
    vl_array *stars;
} Starfield;

typedef struct Point3D {
    float x;
    float y;
    float z;
} Point3D;

typedef struct Bounds {
    int x;
    int y;
} Bounds;

Bounds bounds;

Starfield* get_starfield()
{
    SDL_Surface* sdls = SDL_GetVideoSurface();
    bounds.x = sdls->w;
    bounds.y = sdls->h;
    srand (time(NULL));
    Starfield *sf = malloc (sizeof(Starfield));
    sf->stars = new_array(sizeof(Point3D));
    Point3D p;
    int i;
    for (i = 0; i < 32; i++){
        p.x = rand() % bounds.x;
        p.y = rand() % bounds.y;
        p.z = 2 + rand() % 10;
        add_element(sf->stars, &p); 
    }
    return sf;
}

void draw_starfield(Starfield *sf)
{
   SDL_Surface *b = SDL_GetVideoSurface();
   int len = vl_array_length(sf->stars);
   int i;
   for (i = 0; i < len; i++){
        Point3D *v = get_element(sf->stars, i);
        int color = 0xff77ff;
        draw_pixel((int)v->x, (int)v->y, b, &color); 
   }
} 

/*
 * checks to see if stars are out of bounds; if so, moves them to valid position
 */ 
void bounds_check_and_fix(Point3D *ele)
{   
    if (ele->y < 0){
        ele->x = rand() % bounds.x;
        ele->y = bounds.y - 1;
        ele->z = 2 + rand() % 10;
    } else if (ele->y >= bounds.y){
        ele->x = rand() % bounds.x;
        ele->y = 0;
        ele->z = 2 + rand() % 10;
    } else if (ele->x < 0){
        ele->x = bounds.x - 1;
        ele->y = rand() % bounds.y;
        ele->z = 2 + rand() % 10;
    }else if (ele->x >= bounds.x){
        ele->x = 0;
        ele->y = rand() % bounds.y;
        ele->z = 2 + rand() % 10;
    }
}

void update_starfield(Starfield *sf, int angle)
{
    int len = vl_array_length(sf->stars);
    Vector *v = get_unit_vector(angle); 
    int i;
    for (i = 0; i < len; i++){
        Point3D * ele = get_element(sf->stars, i);
        ele->x -= v->x / ele->z;
        ele->y -= v->y / ele->z;
        bounds_check_and_fix(ele);
    }
    free(v);
}
