#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include "text.h"

typedef struct
{
    int x, y, w, h, act_b, down, up, lmc, mmc;
    int type;
    wchar_t* string;
} Button_t;

typedef struct
{
    int lm, rm, lrad, rrad;
} Cursor_t;

void ListInitialize();

void ListTerminate();

Button_t* ButtonInitialize(int x, int y, int w, int h, int type, wchar_t* s);

void ButtonTerminate(Button_t* b);

void ButtonDraw(Button_t* b, Font_t* f);

void ListDraw(Font_t* f);

void ListCheck();

void ButtonAction(Button_t* b);

void DisplayPanning(Kvad_t* ptr, Display_t* d);

void ScreenInput(Kvad_t* ptr, Display_t* d);

void ButtonText(Button_t* b, wchar_t* s);

#endif