#include "text.h"

Font_t* FontInitialise()
{
    Font_t* f = (Font_t*)malloc(sizeof(Font_t));
    f->character_width = 8;
    f->character_height = 8;
    f->sprite = SpriteInitialize(
        f->character_width, f->character_height, "./src/font.png");
    return f;
}

void FontCharDraw(Font_t* f, wchar_t c, int x, int y)
{
    //SDL_SetRenderDrawColor( m_renderer, rgb[val][0], rgb[val][1], rgb[val][2], 255 );
    wchar_t n = c;     // ~    126
    if(n > 126) n -= 49;    // °    127
    if(n > 127) n -= 848;   // Ё    128
    if(n > 128) n -= 14;    // Ая   129 - 192
    if(n > 192) n -= 1;     // ё    193
    if(n > 193) n -= 7108;  // ‖    194
    if(n > 194) n -= 255;   // №    195
    if(n > 195) n -= 1314;  // ☹☺   196 - 197 
    n -= 33;

    int srcx = (n % 15) * f->character_width;
    int srcy = (n / 15) * f->character_height;
    int dstx = x * f->character_width;
    int dsty = y * f->character_height;
    SpriteDraw(f->sprite, srcx, srcy, dstx, dsty);
}

void FontStringDraw(Font_t* f, int ax, int ay, int w, int h, wchar_t* s)
{
    int cx = ax, cy = ay;
    wchar_t c = *s;
    int i = 0;
    while(c != '\0' && i < w * h)
    {
        c = *s++;
        switch (c)
        {
        case '\n':
            cx = ax;
            cy += 1;
            break;

        default:
            if(cx >= ax + w)
            {
                cx = ax;
                cy += 1;
            }
            if(c != ' ')
                FontCharDraw(f, c, cx, cy);
            cx += 1;
            break;
        }
        
        i++;
    }
}