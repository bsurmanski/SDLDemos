#include <stdlib.h>
#include <SDL/SDL.h>
#include "ship.h"
#include "sprite.h"
#include "render.h"
#include "vl_array.h"
#include "matrix.h"

#define R 0xff0000
#define G 0x00ff00
#define B 0x0000ff
#define K 0x000100

typedef struct Bullet {
    int x;
    int y;
    int w;
    int h;
    float vx;
    float vy;
} Bullet;

struct Ship {
    int x;
    int y;
    float vx;
    float vy;
    float dv;
    int rot;
    int vrot; //current rotation speed
    int drot; //rate of rotation speed
    vl_array* bullets;
    Sprite sprite;
    Sprite rot_buffer;
    SDL_Surface *rainbow; 
};

int shipdef[16][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,R,R,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,R,R,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,R,R,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,R,R,R,R,0,0,0,0,0,0},
    {0,0,0,0,0,0,R,B,B,R,0,0,0,0,0,0},
    {0,0,0,0,0,R,R,B,B,R,R,0,0,0,0,0},
    {0,0,0,0,R,R,B,B,B,B,R,R,0,0,0,0},
    {0,0,0,R,R,R,R,R,R,R,R,R,R,0,0,0},
    {0,0,0,R,R,R,R,K,K,R,R,R,R,0,0,0},
    {0,0,R,R,R,R,K,K,K,K,R,R,R,R,0,0},
    {0,0,R,R,R,K,K,R,R,K,K,R,R,R,0,0},
    {0,0,R,K,K,K,K,R,R,K,K,K,K,R,0,0},
    {0,K,R,K,K,K,K,0,0,K,K,K,K,R,K,0},
    {0,K,K,K,K,K,0,0,0,0,K,K,K,K,K,0},
    {0,K,K,K,0,0,0,0,0,0,0,0,K,K,K,0}
};



Ship* get_ship(int x, int y)
{
    SDL_Surface *fb = SDL_GetVideoSurface();
    int bpp = fb->pitch / fb->w;
    Sprite sprite = {shipdef, 16, 16, bpp};
    Ship* ship = malloc (sizeof(Ship)); 
    ship->x = x;
    ship->y = y;
    ship->vx = 0;
    ship->vy = 0;
    ship->dv = 2;
    ship->rot = 0;
    ship->vrot = 0;
    ship->drot = 5;
    ship->bullets = new_array(sizeof (Bullet));
    ship->sprite = sprite;
    Sprite rotBuffer = {malloc(sizeof(int) * 24 * 24), 24, 24, bpp};
    memset(rotBuffer.pixels, 0, sizeof(int) * 24 *24);
    rotate_sprite(sprite, rotBuffer, 0);
    ship->rot_buffer = rotBuffer;
    ship->rainbow = SDL_CreateRGBSurface(SDL_SWSURFACE, fb->w, fb->h, 8*fb->pitch/fb->w, 0, 0, 0, 0);
    int bytes = fb->h * fb->pitch;
    memset(ship->rainbow->pixels, 0, bytes);
    return ship;
}

void draw_ship(Ship *s)
{
    SDL_Surface *fb = SDL_GetVideoSurface();
    //draw rainbow
    draw_rainbow(s->rainbow, s->x + s->sprite.w/2, s->y + s->sprite.h/2, (int)s->vx, (int)s->vy, 3);
    int ignore = 0x00000000;
    fill_buffer_with_ignore (fb, s->rainbow, &ignore);
    //draw ship
    draw_sprite_with_ignore(s->rot_buffer, fb, s->x, s->y, &ignore);
    int i;
    //draw bullet
    for (i = 0; i < vl_array_length(s->bullets); ++i){
        Bullet *b = (Bullet*) get_element(s->bullets, i);
        int color = 0xffffff;
        draw_pixel(b->x, b->y, fb, &color);
        draw_pixel(b->x + 1, b->y, fb, &color);
        draw_pixel(b->x - 1, b->y, fb, &color);
        draw_pixel(b->x, b->y + 1, fb, &color);
        draw_pixel(b->x, b->y - 1, fb, &color);
    }
}

int atest = 0;

void update_ship(Ship *s)
{
    //update ship
    if (s->vrot){
        s->rot += s->vrot;
        s->rot %= 360;
        if (s->rot < 0) s->rot +=360;
        rotate_sprite(s->sprite, s->rot_buffer, s->rot);
    }
    s->x += (int)s->vx;
    s->y += (int)s->vy;
    if (s->x < 0) s->x = 0;
    if (s->x + s->sprite.w > s->rainbow->w) s->x = s->rainbow->w - s->sprite.w;
    if (s->y < 0) s->y = 0;
    if (s->y + s->sprite.h > s->rainbow->h) s->y = s->rainbow->h - s->sprite.h;

    //update rainbow
    update_rainbow(s->rainbow, s->x + s->sprite.w/2, s->y + s->sprite.h/2, (int) s->vx,  (int)s->vy, 3);

    //update bullets
    int i;
    for (i = 0; i < vl_array_length(s->bullets); ++i){
        Bullet* b = ((Bullet*) get_element(s->bullets, i));
        b->x += (int)b->vx;
        b->y += (int)b->vy;
        if (point_offscreen (s->rainbow, b->x, b->y) || point_offscreen(s->rainbow, b->x + b->w, b->y + b->h)){
            remove_element(s->bullets, b);  
        }
    }
}

void fire_bullet(Ship* s)
{
    Bullet b;
    b.x = s->x + s->sprite.w/2;
    b.y = s->y + s->sprite.h/2;
    b.w = 5;
    b.h = 5;
    Vector *v = get_unit_vector(s->rot);
    b.vx = v->x * 5;
    b.vy = v->y * 5;
    add_element(s->bullets, &b); 
}

int get_ship_vx(Ship *s)
{
    int ret = 0;
    if (s->vx > 0) ret = 1;
    if (s->vx < 0) ret = -1;
    return ret;
}

int get_ship_vy(Ship *s)
{
    int ret = 0;
    if (s->vy > 0) ret = 1;
    if (s->vy < 0) ret = -1;
    return ret;
}

int get_ship_rotation(Ship *s)
{
    return s->rot;
}

void set_ship_direction(Ship *s, int mx, int my)
{
    s->vx = mx * s->dv;
    s->vy = my * s->dv;
}

int get_ship_vr(Ship *s){
    int ret = 0;
    if (s->vrot > 0) ret = 1;
    if (s->vrot < 0) ret = -1;
    return ret;
}

void set_ship_rotation_direction(Ship *s, int rot){
    s->vrot = rot * s->drot;
}

#undef R
#undef G
#undef B
