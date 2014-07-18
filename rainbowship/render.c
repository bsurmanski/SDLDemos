#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include "render.h"
#include "sprite.h"
#include "vl_array.h"
#include "matrix.h"

inline int point_offscreen (SDL_Surface *fb, int x, int y)
{
    return (x > fb->w
            || x < 0
            || y > fb->h
            || y < 0);
}

inline void fill_buffer(SDL_Surface *dest, SDL_Surface *src)
{
    if (dest->w == src->w && dest->h == src->h && dest->pitch == src->pitch){
        memcpy(dest->pixels, src->pixels, src->pitch * src->h);
    }
}

void fill_buffer_with_ignore(SDL_Surface *dest, SDL_Surface *src, void* ignore)
{
    if (dest->w == src->w && dest->h == src->h && dest->pitch == src->pitch){
        size_t bytes = src->h * src->pitch;
        size_t bpp = src->pitch / src->w;
        int i;
        for (i = 0; i < bytes; i+=bpp){
            if (memcmp(src->pixels + i, ignore, bpp)){
                memcpy(dest->pixels + i, src->pixels + i, bpp);
            }
        }
    }
}

inline void clear_buffer(SDL_Surface *fb)
{
    memset (fb->pixels, 0, fb->h * fb->pitch);
}

void draw_line (int x0, int y0, int x1, int y1, SDL_Surface *fb)
{
    int dx = abs(x1-x0), sx = x0<x1 ? 1: -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1: -1;
    int err = (dx>dy ? dx : -dy)/2, e2;
    int pxl = 0xffffff;
    for (;;){
    //draw pixel here
    draw_pixel (x0, y0, fb, &pxl);  
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) {err -= dy; x0 += sx;}
    if (e2 < dy) {err += dx; y0 += sy;}
    }
}

inline void draw_pixel (int x, int y, SDL_Surface *fb, void* value)
{
    int bpp = fb->pitch / fb->w;
    memcpy (fb->pixels + (x * bpp + y * fb->pitch), value, bpp); 
}

void draw_sprite(Sprite s, SDL_Surface *fb, int x, int y)
{

 int i;
 int bpp = fb->pitch / fb->w;
    for (i = 0; i < s.h; i++)
        memcpy(fb->pixels + (x * bpp + ((y + i) * fb->pitch)), s.pixels + i * s.w * bpp, s.w * fb->pitch/fb->w);
}

void draw_sprite_with_ignore(Sprite s, SDL_Surface *fb, int x, int y, void* ignore)
{
    int bpp = fb->pitch / fb->w;
    void *pxlptrSrc, *pxlptrDest;
    int i;
    int j;
    for (i = 0; i < s.h; i++){
        for (j = 0; j < s.w; j++){
            pxlptrSrc = s.pixels + (j + i * s.w) * bpp;
            if (memcmp(pxlptrSrc, ignore, bpp)){
                pxlptrDest = fb->pixels + ((j + x) + (i + y) * fb->w) * bpp;
                memcpy(pxlptrDest, pxlptrSrc, bpp);
            } 
        }
    }
}

void rotate_sprite(Sprite in, Sprite out, int deg)
{
    memset(out.pixels, 0, (out.w * out.h) * out.bpp);
    int midx = in.w / 2.0;
    int midy = in.h / 2.0;
    int midxo = out.w / 2.0;
    int midyo = out.h / 2.0;
    void* inpxl;
    void* outpxl;
    Vector *v = get_vector();
    Matrix *m = get_rotation_matrix(deg);
    int i, j;
    for (i = 0; i < in.h; i++){
        for (j = 0; j < in.w; j++){
            set_vector(v, j - midx, i - midy); 
            apply_matrix(v, m);
            set_vector(v, v->x + midx, v->y + midy);
            if (v->x < 0 || v->x >= in.w || v->y < 0 || v->y >= in.h) continue; 
            inpxl = in.pixels + ((int)(v->x) + (int)(v->y) * in.w) * in.bpp;
            outpxl = out.pixels + (j + i * out.w) * out.bpp; 
            memcpy(outpxl, inpxl, in.bpp);
        }
    }
    free(v);
    free(m);
}
