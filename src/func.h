
#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <time.h>

extern int side;

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
    int shift;
} InputState_t;

//HEXAGONS

typedef struct
{
    int mat, tmp, prs_grav, prs_dist, fld, fld2, dx, dy, st8, dns, clr, stress;
} Cell_t;



//VIDEO

typedef struct
{
    //Kvad_t* k;
    SDL_Rect screen;
    int sf;
    SDL_Rect hshift;
    SDL_Rect screen_shift;
    int angle, scale;
    int grid_x, grid_y, grid_w, grid_h;
    int subz, subn;
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

int mod(int a, int t);

int cycle(int a, int min, int max, int d);

int hdiv(int a, int t);

void swap(int* a, int* b);

int isinrec(int x, int y, int w, int h, int cx, int cy);

int hlog(int a, double p);

void PixelToHex(Display_t* d, int *z, int *n);

void HexToGrid(Display_t* d, int *z, int *n, int centz, int centn);

int floordiv(int dend, int dsor);

#endif
