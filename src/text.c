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

void FontTerminate(Font_t* f)
{
    free(f);
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

void FontNumberDraw(Font_t* f, int ax, int ay, int w, int h, int num)
{
    wchar_t str[240];
    swprintf(str, 240, L"%i", 
    num);
    FontStringDraw(f, ax, ay, w, h, str);
}

void FontUIDraw(Font_t* f, Display_t* d)
{
    wchar_t reading[82];

    FILE* fl1 = NULL;
    fl1 = fopen("./src/ui.txt", "r");
    for(int i = 0; i < 45; i++)
    {
        fgetws(reading, 82, fl1);
        FontStringDraw(f, 0, i, 80, 1, reading); 
    }
    fclose(fl1);

    int x, y;
    MouseToPixels(&x, &y);
    wchar_t str[240];
    swprintf(str, 240, L"угол: %i\nсдвиг по х: %i\nсдвиг по y: %i\nмышь х: %i\nмышь y: %i\n", 
    d->angle, d->hshift.x, d->hshift.y, x, y);
    
    FontStringDraw(f, 1, 28, 15, 16,
    str);
    /*
    FontStringDraw(f, 1, 1, 3, 3, L"/=\\‖o‖\\=/");
    FontStringDraw(f, 5, 1, 3, 3, L"r `   l №");
    FontStringDraw(f, 9, 1, 3, 3, L"o\\ \\ \\ \\o");
    FontStringDraw(f, 1, 5, 3, 3, L" n <ш>iV ");

    FontStringDraw(f, 1, 10, 3, 3, L":::r`/`№ъ");
    FontStringDraw(f, 5, 10, 3, 3, L"n n>>>u u");

    FontStringDraw(f, 1, 18, 3, 4, L"c=\\ // u  o ");
    FontStringDraw(f, 1, 23, 3, 4, L"r\\ |l\\| |l-№");
    FontStringDraw(f, 5, 23, 3, 4, L" Л /‖\\ ‖ c=э");
    FontStringDraw(f, 9, 23, 3, 4, L" ‖ \\‖/ V c=э");
    FontStringDraw(f, 13, 23, 3, 4, L" n qhpdчb u ");
    */
    int r = 8;
    x = 38, y = 22;
    int ang = d->angle / 8;
    int res = d->angle % 8;

    wchar_t angstr[4];
    swprintf(angstr, 4, L"%i", ang * 60);

    FontStringDraw(f, x - 1 + r * 2 - res   , y + res * 2           , 3, 1, angstr);
    swprintf(angstr, 4, L"%i", (ang + 5) % 6 * 60);
    FontStringDraw(f, x - 1 + r - res * 2   , y + r * 2             , 3, 1, angstr);
    swprintf(angstr, 4, L"%i", (ang + 4) % 6 * 60);
    FontStringDraw(f, x - 1 - r - res       , y + r * 2 - res * 2   , 3, 1, angstr);
    swprintf(angstr, 4, L"%i", (ang + 3) % 6 * 60);
    FontStringDraw(f, x - 1 - r * 2 + res   , y - res * 2           , 3, 1, angstr);
    swprintf(angstr, 4, L"%i", (ang + 2) % 6 * 60);
    FontStringDraw(f, x - 1 - r + res * 2   , y - r * 2             , 3, 1, angstr);
    swprintf(angstr, 4, L"%i", (ang + 1) % 6 * 60);
    FontStringDraw(f, x - 1 + r + res       , y - r * 2 + res * 2   , 3, 1, angstr);
}