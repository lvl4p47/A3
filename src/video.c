#include "video.h"

extern int pixelsize;
extern InputState_t inpst;
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

    hexel.x = 0; hexel.y = 0; hexel.w = 5; hexel.h = 5;

    IMG_Init(IMG_INIT_PNG);
    
    s1 = SpriteInitialize(9, 9, "./src/tiles.png");

}

void VideoTerminate()
{
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

void HexelDraw(Display_t* d, int z, int n, int val)
{

    //shx = 0, shy = 0;

    hexel.x = d->screen.x + d->screen.w / 2 + d->hshift.w * 1 +
    (z + d->hshift.x) * hcos(d->angle) + (n + d->hshift.y) * hcos(d->angle + 8);
    hexel.y = d->screen.y + d->screen.h / 2 + d->hshift.h * 1 +
    (z + d->hshift.x) * hsin(d->angle) + (n + d->hshift.y) * hsin(d->angle + 8);

    int srcx = (d->angle % 8) * 9;
    int srcy = val * 9;

    SDL_SetRenderDrawColor( m_renderer, rgb[val][0], rgb[val][1], rgb[val][2], 255 );

    SpriteDraw(s1, srcx, srcy, hexel.x, hexel.y);
    //SDL_RenderFillRect( m_renderer, &hexel );
}

void KvadRender(Kvad_t* ptr, Display_t* d, int x, int y)
{
    SDL_SetRenderDrawColor( m_renderer, rgb[3][0], rgb[3][1], rgb[3][2], 255 );
    //SDL_RenderDrawRect( m_renderer, &d->screen );

    int sx = ptr->width / 2 * 0, sy = ptr->height / 2 * 0;
    for (int i = 0; i < ptr->height; i++)
    {
        for (int j = 0; j < ptr->width; j++)
        {
            switch(ptr->arr[i][j].mat)
            {
            case 10:
                HexelDraw(d, j - sx, i - sy, 3);
                break;

            default:
                HexelDraw(d, j - sx, i - sy, ptr->arr[i][j].mat);
                break;
            }
        }
    }
}

void KvadPanning(Kvad_t* ptr, Display_t* d)
{
    int wx, wy, pwx, pwy;
    int x, y;

        if(inpst.mouse.pressed)
        {
            MouseToHex(d, &x, &y);

            if(inpst.mouse.lmc)
                KvadSetBlob(ptr, x, y, 1, 1);
            if(inpst.mouse.rmc)
                KvadSetBlob(ptr, x, y, 0, 1);
            if(inpst.mouse.mmc)
            {
                MouseToPixels(&wx, &wy);
                PMouseToPixels(&pwx, &pwy);

                d->screen_shift.x = d->screen_shift.w + wx - pwx;
                d->screen_shift.y = d->screen_shift.h + wy - pwy;

                d->hshift.x = d->screen_shift.x, d->hshift.y = d->screen_shift.y;
                PixelToHex(d, &d->hshift.x, &d->hshift.y);

                d->hshift.w = d->screen_shift.x -
                (d->hshift.x) * hcos(d->angle) - (d->hshift.y) * hcos(d->angle + 8);
                d->hshift.h = d->screen_shift.y -
                (d->hshift.x) * hsin(d->angle) - (d->hshift.y) * hsin(d->angle + 8);
            }
        }
        if(inpst.mouse.up)
        {
                d->screen_shift.w = d->screen_shift.x;
                d->screen_shift.h = d->screen_shift.y;
                printf( "UP\n");
        }
        if(inpst.mouse.wheel)
        {
            d->angle = (d->angle + 48 + inpst.mouse.scroll) % 48;
            inpst.mouse.wheel = 0;

            d->screen_shift.x = 1 * d->hshift.w +
            (d->hshift.x) * hcos(d->angle) + (d->hshift.y) * hcos(d->angle + 8);
            d->screen_shift.y = 1 * d->hshift.h +
            (d->hshift.x) * hsin(d->angle) + (d->hshift.y) * hsin(d->angle + 8);
            //printf( "screen_shift %i:%i \n",  screen_shift.x, screen_shift.y );

            inpst.mouse.px = inpst.mouse.x;
            inpst.mouse.py = inpst.mouse.y;

            d->screen_shift.w = d->screen_shift.x;
            d->screen_shift.h = d->screen_shift.y;

        }
}

Display_t* DisplayInitialize(int x, int y, int w, int h) // 17 1 43 43
{
    Display_t* d = (Display_t*)malloc(sizeof(Display_t));
    d->screen.x = 8 * x;    d->screen.y = 8 * y;
    d->screen.w = 8 * w;    d->screen.h = 8 * h;
    d->screen_shift.x = 0;  d->screen_shift.y = 0;
    d->screen_shift.w = 0;  d->screen_shift.h = 0;
    d->hshift.x = 0;        d->hshift.y = 0;
    d->hshift.w = 0;        d->hshift.h = 0;
    d->angle = 0;

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
    //s->texture = IMG_LoadTexture(m_renderer, "tiles.png");

    //SDL_Surface* loadedSurface = IMG_Load( "/home/marat/Programming/A3/src/tiles.png" );
    s->texture = SDL_CreateTextureFromSurface( m_renderer, 
    IMG_Load(file) );

    return s;
}

void SpriteTerminate(Sprite_t* s)
{
    free(s);
}

void SpriteDraw(Sprite_t* s, int x1, int y1, int x2, int y2)
{
    s->source.x = x1;            s->source.y = y1;
    s->destination.x = x2;       s->destination.y =  y2;
    SDL_RenderCopy(m_renderer, s->texture, &s->source, &s->destination);
}
