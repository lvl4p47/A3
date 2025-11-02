#ifndef VIDEO_H_INCLUDED
#define VIDEO_H_INCLUDED

#include "hexagons.h"

extern Display_t** displaylist;
extern Sprite_t* f21;
extern int minimap_speed;

void point(int x, int y, int r, int g, int b);

void VideoInitialize();

void VideoTerminate();

void ScreenClear();

void ScreenRefresh();

void HexelDraw(Display_t* d, int z, int n, Cell_t* c, int b_ui);

void DotsDraw(Display_t* d, int z, int n, int amount, int alpha);

void HexelDrawOnUI(int x, int y, int mat, int ang, int b_ui);

void KvadRender(Kvad_t* ptr, Display_t* d, int x, int y, int b_ui);

Display_t* DisplayInitialize(int x, int y, int w, int h, int angle, int scale);

void DisplayTerminate(Display_t* d);

Sprite_t* SpriteInitialize(int width, int height, const char* file);

void SpriteTerminate(Sprite_t* s);

void SpriteDraw(Sprite_t* s, int x1, int y1, int x2, int y2, int color, int alpha);

void ResizeWindow(int ps);

void ToggleWindow(int b_min);

void DisplayScan(Kvad_t* ptr, Display_t* d, int b_ui, int scale_selection);

void DisplayListDraw(Kvad_t* ptr, int b_ui);

void DisplayListInitialize();

void DisplayListTerminate();

void EntityDraw(Kvad_t* ptr, Display_t* d, Entity_t* p_e);

#endif
