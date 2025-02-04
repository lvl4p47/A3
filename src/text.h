#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "video.h"

Font_t* FontInitialise();

void FontCharDraw(Font_t* f, wchar_t c, int x, int y);

void FontStringDraw(Font_t* f, int ax, int ay, int w, int h, wchar_t* s);

//, wchar_t* s

#endif