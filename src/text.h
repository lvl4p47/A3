#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "video.h"

Font_t* FontInitialise();

void FontTerminate(Font_t* f);

void FontCharDraw(Font_t* f, wchar_t c, int x, int y, int color);

void FontCharFill(Font_t* f, wchar_t c, int x, int y, int w, int h, int color);

void FontStringDraw(Font_t* f, int ax, int ay, int w, int h, wchar_t* s, int color);

void FontStringShiftDraw(Font_t* f, int ax, int ay, int w, int h, wchar_t* s, int color, int shift_y);

int StringLinesCount(int w, wchar_t* s);

void FontNumberDraw(Font_t* f, int ax, int ay, int w, int h, int num, int color, int sign, int set_width);

#endif