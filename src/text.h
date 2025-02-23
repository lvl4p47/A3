#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "video.h"

Font_t* FontInitialise();

void FontTerminate(Font_t* f);

void FontCharDraw(Font_t* f, wchar_t c, int x, int y);

void FontStringDraw(Font_t* f, int ax, int ay, int w, int h, wchar_t* s);

void FontNumberDraw(Font_t* f, int ax, int ay, int w, int h, int num);

void FontUIDraw(Font_t* f, Display_t* d);

#endif