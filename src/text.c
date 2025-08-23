#include "text.h"


Font_t* FontInitialise()
{
    Font_t* f = (Font_t*)malloc(sizeof(Font_t));
    f->character_width = 8;
    f->character_height = 8;
    f->sprite = SpriteInitialize(
        f->character_width, f->character_height, "../media/font.png");
    return f;
}

void FontTerminate(Font_t* f)
{
    free(f);
}

void FontCharDraw(Font_t* f, wchar_t c, int x, int y, int color)
{
    //SDL_SetRenderDrawColor( m_renderer, rgb[val][0], rgb[val][1], rgb[val][2], 255 );
    wchar_t n = c;          // ~    126
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
    SpriteDraw(f->sprite, srcx, srcy, dstx, dsty, color);

}

void FontCharFill(Font_t* f, wchar_t c, int x, int y, int w, int h, int color)
{
    int cx = x, cy = y;
    int i = 0;
    while(i < w * h)
    {
        if(cx >= x + w)
            {
                cx = x;
                cy += 1;
            }
            FontCharDraw(f, c, cx, cy, color);
            cx += 1;
        i++;
    }
}

void FontStringDraw(Font_t* f, int ax, int ay, int w, int h, wchar_t* s, int color)
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
                FontCharDraw(f, c, cx, cy, color);
            cx += 1;
            break;
        }
        
        i++;
    }
}

void FontStringShiftDraw(Font_t* f, int ax, int ay, int w, int h, wchar_t* s, int color, int shift_y)
{

    int cx = ax, cy = ay;
    wchar_t c = *s;
    int i = 0;
    while(c != '\0' && i < w * shift_y)
    {
        c = *s++;
        switch (c)
        {
        case '\n':
            i += w - (cx - ax) - 1;
            cx = ax;
            cy += 1;
            
            break;

        default:
            if(cx >= ax + w)
            {
                cx = ax;
                cy += 1;
            }
            cx += 1;
            break;
        }
        
        i++;
    }
    while(c != '\0' && i < w * (h + shift_y))
    {
        c = *s++;
        switch (c)
        {
        case '\n':
            i += w - (cx - ax) - 1;
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
                FontCharDraw(f, c, cx, cy - shift_y, color);
            cx += 1;
            break;
        }
        
        i++;
    }
}

int StringLinesCount(int w, wchar_t* s)
{

    int cx = 0, cy = 0, i = 0;
    wchar_t c = *s;
    while(c != '\0' && i < 10000)
    {
        c = *s++;
        switch (c)
        {
        case '\n':
            cx = 0;
            cy += 1;
            
            break;

        default:
            if(cx >= w)
            {
                cx = 0;
                cy += 1;
            }
            cx += 1;
            break;
        }
        i++;
    }
    return cy + 1;
}

void FontNumberDraw(Font_t* f, int ax, int ay, int w, int h, int num, int color, int sign, int set_width)
{
    wchar_t str[240];
    if(sign && set_width)swprintf(str, 240, L"%+0*i", w, num);
    else if(sign && !set_width)swprintf(str, 240, L"%+i", w, num);
    else if(!sign && set_width)swprintf(str, 240, L"%0*i", w, num);
    else swprintf(str, 240, L"%i", w, num);
    FontStringDraw(f, ax, ay, w, h, str, color);
}

