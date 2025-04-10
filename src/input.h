#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "audio.h"

typedef struct
{
    int x, y, pressed, down, up, lmc, mmc, rmc, px, py, wheel, scroll;
} MouseState_t;

typedef struct
{
    MouseState_t mouse;
    int left, right, up, down;
    int vx, vy, delete, insertA, insertB;
    int jump;
    int shift;
} InputState_t;

extern int b_quit;
extern SDL_Event event;
extern int pixelsize;
extern InputState_t inpst;

void InputInitialize();

void InputRegister();

void InputUpdate();

void InputTerminate();

void MouseToPixels(int *x, int *y);

void PMouseToPixels(int *x, int *y);

void MouseResetPrev();

void MouseToGrid(int *x, int *y);

void MouseToHex(Display_t* d, int *z, int *n);

#endif
