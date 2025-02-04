
#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

#include <SDL2/SDL.h>
#include "SDL_image.h"
#include <stdlib.h>
#include <time.h>

//FUNC

//INPUT

typedef struct
{
    int x, y, pressed, down, up, lmc, mmc, rmc, px, py, wheel, scroll;
} MouseState_t;

typedef struct
{
    MouseState_t mouse;
    int vx, vy;
} InputState_t;

//HEXAGONS

typedef struct
{
    int mat, num, fld, vel;
} Cell_t;

typedef struct
{
    int width, height;
    Cell_t** arr;
} Kvad_t;

//VIDEO

typedef struct
{
    //Kvad_t* k;
    SDL_Rect screen;
    int sf;
    SDL_Rect hshift;
    SDL_Rect screen_shift;
    int angle;
} Display_t;

typedef struct
{
    SDL_Rect source, destination;
    SDL_Texture* texture;

} Sprite_t;

//FONT

typedef struct
{
    int character_width, character_height;
    Sprite_t* sprite;
} Font_t;


int hsin(int a);

int hcos(int a);

int64_t hmax(int64_t a, int64_t b);

int64_t hmin(int64_t a, int64_t b);

void PixelToHex(Display_t* d, int *z, int *n);

#endif
