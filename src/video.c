#include "video.h"

extern int pixelsize;

SDL_Window * m_window;
SDL_Renderer * m_renderer;
int rgb[8][3] = {
                {0, 0, 0}, // Black
                {255, 0, 0}, // Red
                {0, 255, 0}, // Green
                {0, 0, 255}, // Blue
                {0, 255, 255}, // Cyan
                {255, 0, 255}, // Magenta
                {255, 255, 0},  // Yellow
                {255, 255, 255}  // White
                };
SDL_Rect hexel;
Sprite_t* s1 = NULL;
Sprite_t* s2 = NULL;

void point(int x, int y, int r, int g, int b)
{
    SDL_SetRenderDrawColor(m_renderer, r, g, b, 100);
    SDL_RenderDrawPoint(m_renderer, x, y);
}

void VideoInitialize()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(
                    640 * pixelsize, 
                    360 * pixelsize, 
                    SDL_WINDOW_SHOWN, 
                    &m_window, 
                    &m_renderer 
                    );
    SDL_SetWindowBordered(m_window, SDL_FALSE);
    SDL_RenderSetIntegerScale(m_renderer, SDL_TRUE);
    SDL_RenderSetLogicalSize(m_renderer, 640, 360);

    hexel.x = 0; hexel.y = 0; hexel.w = 9; hexel.h = 9;

    IMG_Init(IMG_INIT_PNG);
    
    s1 = SpriteInitialize(9, 9, "./src/tiles.png");
    s2 = SpriteInitialize(9, 9, "./src/ui_tiles.png");
}

void VideoTerminate()
{
    SpriteTerminate(s1);
    SpriteTerminate(s2);

    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );
    SDL_Quit();
}

void ScreenClear()
{
    SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, 255 );
    SDL_RenderClear( m_renderer );
}

void ScreenRefresh()
{
    SDL_RenderPresent( m_renderer );
}

void HexelDraw(Display_t* d, int z, int n, Cell_t* c, int b_ui)
{
    hexel.x = d->screen.x + d->screen.w / 2 + d->hshift.w * 1 +
    (z + d->hshift.x) * hcos(d->angle) + (n + d->hshift.y) * hcos(d->angle + 8);
    hexel.y = d->screen.y + d->screen.h / 2 + d->hshift.h * 1 +
    (z + d->hshift.x) * hsin(d->angle) + (n + d->hshift.y) * hsin(d->angle + 8);

    int srcx = d->angle * 9;
    int srcy = c->mat * 9;

    if(b_ui == 1)
    {
        srcx = d->angle * 9;
        srcy = (mod(c->dns, 11)) * 9;
        SpriteDraw(s2, srcx, srcy, hexel.x, hexel.y, c->clr);
    }
    else
    {
        srcx = d->angle * 9;
        srcy = c->mat * 9;
        SpriteDraw(s1, srcx, srcy, hexel.x, hexel.y, c->clr);
    }
}

void KvadRender(Kvad_t* ptr, Display_t* d, int x, int y, int b_ui)
{
    SDL_SetRenderDrawColor( m_renderer, rgb[3][0], rgb[3][1], rgb[3][2], 255 );
    for (int i = 0; i < ptr->height; i++)
    {
        for (int j = 0; j < ptr->width; j++)
        {
            HexelDraw(d, j, i, &ptr->arr[i][j], b_ui);
        }
    }
}



Display_t* DisplayInitialize(int x, int y, int w, int h) // 17 1 43 43
{
    Display_t* d = (Display_t*)malloc(sizeof(Display_t));
    d->screen.x = 8 * x;    d->screen.y = 8 * y;
    d->screen.w = 8 * w;    d->screen.h = 8 * h;
    d->screen_shift.x = 0;  d->screen_shift.y = 0;
    d->screen_shift.w = 0;  d->screen_shift.h = 0;
    d->hshift.x = -32;        d->hshift.y = -32;
    d->hshift.w = 0;        d->hshift.h = 0;
    d->angle = 4;
    
    d->screen_shift.x = 1 * d->hshift.w +
        (d->hshift.x) * hcos(d->angle) + (d->hshift.y) * hcos(d->angle + 8);
        d->screen_shift.y = 1 * d->hshift.h +
        (d->hshift.x) * hsin(d->angle) + (d->hshift.y) * hsin(d->angle + 8);


    return d;
}

void DisplayTerminate(Display_t* d)
{
    free(d);
}

Sprite_t* SpriteInitialize(int width, int height, const char* file)
{
    Sprite_t* s = (Sprite_t*)malloc(sizeof(Sprite_t));
    s->source.x = 0;            s->source.y = 0;
    s->source.w = width;        s->source.h = height;
    s->destination.x = 0;       s->destination.y = 0;
    s->destination.w = width;   s->destination.h = height;
    
    s->texture = SDL_CreateTexture(m_renderer, 0, 
        SDL_TEXTUREACCESS_STATIC, s->source.w, s->source.h);
    
    s->texture = SDL_CreateTextureFromSurface( m_renderer, 
    IMG_Load(file) );
    
    //SDL_SetTextureBlendMode(s->texture, SDL_BLENDMODE_ADD);

    return s;
}

void SpriteTerminate(Sprite_t* s)
{
    free(s);
}

void SpriteDraw(Sprite_t* s, int x1, int y1, int x2, int y2, int color)
{
    s->source.x = x1;            s->source.y = y1;
    s->destination.x = x2;       s->destination.y =  y2;
    SDL_SetTextureColorMod(s->texture, rgb[color][0], rgb[color][1], rgb[color][2]);
    SDL_RenderCopy(m_renderer, s->texture, &s->source, &s->destination);
}

void ResizeWindow(int ps)
{
    SDL_SetWindowSize(m_window, 640 * ps, 360 * ps);
    SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowInputFocus(m_window);
    pixelsize = ps;
}

void ToggleWindow(int b_min)
{
    if(b_min) SDL_MinimizeWindow(m_window);
    else SDL_MaximizeWindow(m_window);
}