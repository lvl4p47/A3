
#include "func.h"

//FUNC

//INPUT
SDL_Event event;
int quit;
int pixelsize = 3;
InputState_t inpst;

//HEXAGONS
int side = 48;

//VIDEO

int trig[48] =
{
        0, 1, 2, 3, 4, 5, 6, 6, 7, 7, 8, 8,
        8, 8, 8, 7, 7, 6, 6, 5, 4, 3, 2, 1,
        0,-1,-2,-3,-4,-5,-6,-6,-7,-7,-8,-8,
       -8,-8,-8,-7,-7,-6,-6,-5,-4,-3,-2,-1
};

int hsin(int a)
{
    return trig[a % 48];
}

int hcos(int a)
{
    return trig[(a + 12) % 48];
}

int64_t hmax(int64_t a, int64_t b) {
	if (a > b) return a;
	else return b;
}

int64_t hmin(int64_t a, int64_t b) {
	if (a < b) return a;
	else return b;
}

void PixelToHex(Display_t* d, int *z, int *n)
{
    int a = d->angle;
    int det = hcos(a) * hsin(a + 8) - hsin(a) * hcos(a + 8);

    int x, y, dz, dn, dx, dy;
    x = *z, y = *n;

    *z = ( x * hsin(a + 8) - y * hcos(a + 8) ) / det;
    *n = (-x * hsin(  a  ) + y * hcos(  a  ) ) / det;
}
