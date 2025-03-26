#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include "text.h"

extern int b_pause, b_step;
extern int b_ui;
extern uint64_t t_f;
extern uint64_t t_s;

typedef struct
{
    int x, y, w, h, act_b, down, up, lmc, mmc;
    int type;
    int text;
    int b_grab;
    wchar_t* string;
    wchar_t* sscndr;
} Button_t;

typedef struct
{
    int x, y, w, h, act_b, down, up, lmc, mmc;
    int a, d, c;
    int type;
    int b_grab;
} Slider_t;

typedef struct
{
    int lm, rm, lrad, rrad;
} Cursor_t;

typedef struct
{
    int x, y, w, h;
    int list_begin, list_end;
    int mat_from, mat_to, cond_num;
} RulesEditor_t;

typedef struct
{
    int x, y, w, h;
} ui_rectangle_t;

typedef struct
{
    int x, y, w, h, shift;
    wchar_t* s;
    
} InfoBox_t;

void InterfaceInitialize();

void InterfaceTerminate();

void InterfaceDraw(Font_t* f, Display_t* d);

void InterfaceUpdate();

void ButtonListInitialize();

void ButtonListTerminate();

Button_t* ButtonInitialize(int x, int y, int w, int h, int type, wchar_t* s, wchar_t* ss);

void ButtonTerminate(Button_t* b);

void ButtonDraw(Button_t* b, Font_t* f);

void ButtonListDraw(Font_t* f);

void ButtonListCheck();

void ButtonDown(Button_t* b);

void ButtonUp(Button_t* b);

void DisplayPanning(Kvad_t* ptr, Display_t* d);

void ScreenInput(Kvad_t* ptr, Display_t* d);

void ButtonText(Button_t* b, int n);

Slider_t* SliderInitialize(int x, int y, int w, int h, int a, int d, int c, int type);

void SliderTerminate(Slider_t* s);

void SliderListInitialize();

void SliderListTerminate();

void SliderDraw(Slider_t* s, Font_t* f);

void SliderListDraw(Font_t* f);

void SliderListCheck();

void SliderDown(Slider_t* s, int c);

void SliderUp(Slider_t* s);

void FontUIDraw(Font_t* f, Display_t* d);

void FontRulesEditorDraw(Font_t* f);

void FontRulesEditorNeighborsDraw(Font_t* f, int x, int y, int w, int h, int n);

void FontPrintToInfoBox(Font_t* f);

void InfoBoxUpdate();

#endif