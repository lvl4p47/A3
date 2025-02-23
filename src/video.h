#ifndef VIDEO_H_INCLUDED
#define VIDEO_H_INCLUDED

#include "hexagons.h"

void point(int x, int y, int r, int g, int b);

void VideoInitialize();

void VideoTerminate();

void ScreenClear();

void ScreenRefresh();

void HexelDraw(Display_t* d, int z, int n, Cell_t* c);

void KvadRender(Kvad_t* ptr, Display_t* d, int x, int y);

Display_t* DisplayInitialize(int x, int y, int w, int h);

void DisplayTerminate(Display_t* d);

Sprite_t* SpriteInitialize(int width, int height, const char* file);

void SpriteTerminate(Sprite_t* s);

void SpriteDraw(Sprite_t* s, int x1, int y1, int x2, int y2);

#endif
