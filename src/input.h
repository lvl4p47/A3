#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "func.h"

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
