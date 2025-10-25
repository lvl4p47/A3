#include "video.h"

extern int pixelsize;

const int displaylistsize = 1;

Display_t** displaylist;

int scale_selection = 0;
int selection_time = 0;
int minimap_speed = 0;

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
int color_cycle[6] = {1, 6, 2, 4, 3, 5};
SDL_Rect hexel;
Sprite_t* s1;
Sprite_t* s2;
Sprite_t* f21;

void point(int x, int y, int r, int g, int b)
{
    SDL_SetRenderDrawColor(m_renderer, r, g, b, 100);
    SDL_RenderDrawPoint(m_renderer, x, y);
}

void VideoInitialize()
{
    s1 = NULL, s2 = NULL;
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
    SDL_RenderSetVSync(m_renderer, SDL_FALSE);

    hexel.x = 0; hexel.y = 0; hexel.w = 9; hexel.h = 9;

    IMG_Init(IMG_INIT_PNG);
    
    s1 = SpriteInitialize(9, 9, "../media/tiles.png");
    s2 = SpriteInitialize(9, 9, "../media/ui_tiles.png");
    f21 = SpriteInitialize(8, 8, "../media/font.png");
    
    if(s1 == NULL) printf("\n tiles.png not found");
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
        // srcy = (mod(1 + c->dx + 1 + (c->dy + 1) * 3, 11)) * 9;
        // srcy = (mod(c->dns, 11)) * 9;
        srcy = (mod(c->org + 1, 49)) * 9;
        // srcy = (mod(c->mat, 11)) * 9;
        // srcy = (mod(c->stress + 1, 49)) * 9;
        SpriteDraw(s2, srcx, srcy, hexel.x, hexel.y, c->clr);
    }
    else
    {
        srcx = d->angle * 9;
        srcy = c->mat * 9;
        SpriteDraw(s1, srcx, srcy, hexel.x, hexel.y, c->clr);
    }
}

void DotsDraw(Display_t* d, int z, int n, int amount)
{
    hexel.x = d->screen.x + d->screen.w / 2 + d->hshift.w * 1 +
    (z + d->hshift.x) * hcos(d->angle) + (n + d->hshift.y) * hcos(d->angle + 8);
    hexel.y = d->screen.y + d->screen.h / 2 + d->hshift.h * 1 +
    (z + d->hshift.x) * hsin(d->angle) + (n + d->hshift.y) * hsin(d->angle + 8);

    int srcx;
    int srcy;

    srcx = d->angle * 9;
    if(amount == 0) srcy = 0;
    else
    {
        srcy = (1 + mod(amount - 1, 49)) * 9;
    }
    SpriteDraw(s2, srcx, srcy, hexel.x, hexel.y, 7); // color_cycle[mod( (amount - 1) / 10 + 3, 6)]
}

void HexelDrawOnUI(int x, int y, int mat, int ang, int b_ui)
{
    int srcx = ang * 9;
    int srcy = mat * 9;
    
    if(b_ui == 1)
    {
        srcy = mod(st8_dns_clr[mat][1], 11) * 9;
        SpriteDraw(s2, srcx, srcy, x * 8, y * 8, st8_dns_clr[mat][2]);
    }
    else
    {
        srcy = mat * 9;
        SpriteDraw(s1, srcx, srcy, x * 8, y * 8, st8_dns_clr[mat][2]);
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
            if(&ptr->arr[i][j] == NULL) printf("\n kvad render error");
        }
    }
}

Display_t* DisplayInitialize(int x, int y, int w, int h, int angle, int scale)
{
    Display_t* d = (Display_t*)malloc(sizeof(Display_t));
    d->screen.x = 8 * x;    d->screen.y = 8 * y;
    d->screen.w = 8 * w;    d->screen.h = 8 * h;
    d->screen_shift.x = 0;  d->screen_shift.y = 0;
    d->screen_shift.w = 0;  d->screen_shift.h = 0;
    
    d->hshift.x = side / 2 / scale;   d->hshift.y = side / 2 / scale;
    d->hshift.w = 0;        d->hshift.h = 0;
    d->angle = angle;       d->scale = scale;
    d->grid_x = x;          d->grid_y = y;
    d->grid_w = w;          d->grid_h = h;
    d->subz = 0;            d->subn = 0;
    
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
    Sprite_t* s = NULL;
    s = (Sprite_t*)malloc(sizeof(Sprite_t));
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

void DisplayScan(Kvad_t* ptr, Display_t* d, int b_ui, int scale_selection)
{
    int cz, cn;
    int corner_z[4], corner_n[4];
    
    int border = 4;
    
    corner_z[0] = d->screen.x               + border ; 
    corner_n[0] = d->screen.y + border               ;
    corner_z[1] = d->screen.x + d->screen.w - border ; 
    corner_n[1] = d->screen.y + border               ;
    corner_z[2] = d->screen.x + d->screen.w - border ; 
    corner_n[2] = d->screen.y + d->screen.h - border ;
    corner_z[3] = d->screen.x               + border ; 
    corner_n[3] = d->screen.y + d->screen.h - border ;
    
    for(int i = 0; i < 4; i++)
        PixelToHex(d, &corner_z[i], &corner_n[i]);
    
    int zmin, zmax, nmin, nmax;
	zmin = hmin(hmin(corner_z[0], corner_z[1]) , hmin(corner_z[2], corner_z[3])),
	zmax = hmax(hmax(corner_z[0], corner_z[1]) , hmax(corner_z[2], corner_z[3]));

	int nz[4], nn[4];
	for(int i = 0; i < 4; i++) {
        if(corner_z[i] == zmin && corner_z[mod(i + 1, 4)] > corner_z[i]) {
            for(int j = 0; j < 4; j++) {
                nz[mod(3 + j, 4)] = corner_z[mod(i + j, 4)];
                nn[mod(3 + j, 4)] = corner_n[mod(i + j, 4)];
            }
            i = 3;
        }
    }
    for(int i = 0; i < 4; i++) {
                corner_z[i] = nz[i];
                corner_n[i] = nn[i];
    }

	int y0, y1, y2, y3;
	int sclz, scln, quantity;

	for (int j = zmin; j <= zmax; j++) {
        if(corner_z[0] == corner_z[3]) y0 = hmin(corner_n[0], corner_n[3]);
        else y0 = corner_n[3]
         + ( (j - corner_z[3]) * (corner_n[0] - corner_n[3]) / (corner_z[0] - corner_z[3]));
        if(corner_z[1] == corner_z[0]) y1 = hmin(corner_n[1], corner_n[0]);
        else y1 = corner_n[0]
         + ( (j - corner_z[0]) * (corner_n[1] - corner_n[0]) / (corner_z[1] - corner_z[0]));
        if(corner_z[2] == corner_z[1]) y2 = hmax(corner_n[2], corner_n[1]);
        else y2 = corner_n[1]
         + ( (j - corner_z[1]) * (corner_n[2] - corner_n[1]) / (corner_z[2] - corner_z[1]));
        if(corner_z[3] == corner_z[2]) y3 = hmax(corner_n[3], corner_n[2]);
        else y3 = corner_n[2]
         + ( (j - corner_z[2]) * (corner_n[3] - corner_n[2]) / (corner_z[3] - corner_z[2]));

        nmin = hmax(y0, y1);
        nmax = hmin(y2, y3);

        for (int i = nmin; i <= nmax; i++) {
            if(d->scale < 0) {

                sclz = hdiv((j + d->subz), (-d->scale)) + 0;
                scln = hdiv((i + d->subn), (-d->scale)) + 0;
                
                int s = -d->scale, s1 = s, s2 = 2 * s;
                int x = mod(j, s);
                int y = mod(i, s);
                if(mod(s , 3) == 0) s1++, s2++;
                int conds[5] = {
                    x + 2 * y < s1,
                    2 * x + y <= s1,
                    x - y < 0,
                    x + 2 * y < s2,
                    2 * x + y <= s2
                };

                if      (conds[0] == 1 && conds[1] == 1);
                else if (conds[2] == 0 && conds[3] == 1) sclz++;
                else if (conds[4] == 1) scln++;
                else    sclz++, scln++;

            }
            if(d->scale == 1)
            {
                sclz = j;
                scln = i;
            }
            if(d->scale <= 1)
                HexelDraw(d, j, i, KvadGetHexel(ptr, sclz, scln), b_ui);
            else
            {
                quantity = 0;
                sclz = (j) * d->scale;
                scln = (i) * d->scale;
                sclz += mod((scale_selection), d->scale);
                scln += mod((scale_selection) / d->scale, d->scale);
                for(int c = 0; c < d->scale; c++)
                {
                    for(int v = 0; v < d->scale; v++)
                    {
                        if(KvadGetHexel(ptr, sclz + c, scln + v)->mat != 0)
                        {
                            quantity++;
                        }
                    }
                }
                DotsDraw(d, j, i, quantity);
            }
		}
	}
}

void DisplayListDraw(Kvad_t* ptr, int b_ui)
{
    for (int i = 0; i < displaylistsize; i++)
	{
        DisplayScan(ptr, displaylist[i], b_ui, scale_selection);
    }
    if(minimap_speed != 0)
    {
        int max_time = 5;
        selection_time = cycle(selection_time, 0, max_time, minimap_speed);
        if(selection_time == max_time)
        {
            scale_selection = cycle(scale_selection, 0, 15, 7);
        }
    }
    else selection_time = 0;
    
    EntityDraw(ptr, displaylist[0], e1);
}

void DisplayListInitialize()
{
    displaylist = (Display_t**)malloc(displaylistsize * sizeof(Display_t*));

	for (int i = 0; i < displaylistsize; i++)
	{
		displaylist[i] = NULL;
    }
    
    displaylist[0] = DisplayInitialize(19, 1, 43, 43, 4, 1);
    // displaylist[1] = DisplayInitialize(63, 1, 16, 22, 4, 4);
}

void DisplayListTerminate()
{
    for(int i = 0; i < displaylistsize; i++)
    {
        DisplayTerminate(displaylist[i]);
    }
    free(displaylist);
}


void EntityDraw(Kvad_t* ptr, Display_t* d, Entity_t* p_e)
{
    int z, n;
    z = p_e->z;
    n = p_e->n;
    
    // z = 10, n = 10;
    
    if(d->scale >= 1)
    {
        z /= d->scale;
        n /= d->scale;
    }
    else
    {
        z *= -d->scale;
        n *= -d->scale;
        z += p_e->subz * -d->scale / p_e->magn;
        n += p_e->subn * -d->scale / p_e->magn;
    }
    // z /= p_e->magn;
    // n /= p_e->magn;

    
    
    DotsDraw(d, z, n, 10);
    // HexelDraw(d, z, n, KvadGetHexel(ptr, z, n), 0);
}