#include "input.h"

int b_quit;
SDL_Event event;
int pixelsize = 2;
InputState_t inpst;

void InputInitialize()
{
    b_quit = 0;

    inpst.mouse.pressed = 0;
    inpst.mouse.down    = 0;
    inpst.mouse.up      = 0;
    inpst.mouse.px      = 0;
    inpst.mouse.py      = 0;
    inpst.mouse.lmc     = 0;
    inpst.mouse.rmc     = 0;
    inpst.mouse.wheel   = 0;
    inpst.mouse.scroll  = 0;
    inpst.mouse.x       = 0;
    inpst.mouse.y       = 0;

    inpst.vx = 0;
    inpst.vy = 0;
}

void InputRegister()
{
    inpst.mouse.down = 0;
    inpst.mouse.up   = 0;
    while( SDL_PollEvent( &event ) )
    {
        inpst.vx = 0;
        inpst.vy = 0;

        switch( event.type ){
        case SDL_KEYDOWN:
            switch( event.key.keysym.sym ){
                case SDLK_LEFT:
                    inpst.vx -= 1;
                    break;
                case SDLK_RIGHT:
                    inpst.vx += 1;
                    break;
                case SDLK_UP:
                    inpst.vy += 1;
                    break;
                case SDLK_DOWN:
                    inpst.vy -= 1;
                    break;
                default:
                    break;
            }
            break;

        case SDL_KEYUP:
            //printf( "Key release detected\n" );
            break;

        case SDL_QUIT:
            b_quit = 1;
            break;

        case SDL_MOUSEBUTTONDOWN:
            inpst.mouse.down = 1;
            switch( event.button.button ){
                case SDL_BUTTON_LEFT:
                    inpst.mouse.lmc = 1;
                    break;
                case SDL_BUTTON_MIDDLE:
                    inpst.mouse.mmc = 1;
                    SDL_GetMouseState(&inpst.mouse.px, &inpst.mouse.py);
                    break;
                case SDL_BUTTON_RIGHT:
                    inpst.mouse.rmc = 1;
                    break;
            }

            inpst.mouse.pressed = 1;
            SDL_GetMouseState(&inpst.mouse.x, &inpst.mouse.y);
            
            //
            break;
        case SDL_MOUSEBUTTONUP:
            inpst.mouse.up   = 1;
            
            switch( event.button.button ){
                case SDL_BUTTON_LEFT:
                    inpst.mouse.lmc = 0;
                    break;
                case SDL_BUTTON_MIDDLE:
                    inpst.mouse.mmc = 0;
                    //inpst.mouse.px = inpst.mouse.x;
                    //inpst.mouse.py = inpst.mouse.y;
                    break;
                case SDL_BUTTON_RIGHT:
                    inpst.mouse.rmc = 0;
                    break;
            }
            if(inpst.mouse.lmc == 0 && inpst.mouse.mmc == 0 && inpst.mouse.rmc == 0)
                inpst.mouse.pressed = 0;

            SDL_GetMouseState(&inpst.mouse.x, &inpst.mouse.y);
            

            break;
        case SDL_MOUSEWHEEL:
            inpst.mouse.wheel = 1;
            inpst.mouse.scroll = event.wheel.y;
            SDL_GetMouseState(&inpst.mouse.x, &inpst.mouse.y);
            break;



        default:
            break;
        }
    }
}

void InputUpdate()
{
    if ( inpst.mouse.pressed )
    {
        SDL_GetMouseState(&inpst.mouse.x, &inpst.mouse.y);
    }
}

void InputTerminate()
{

}

void MouseToPixels(int *x, int *y)
{
    *x = inpst.mouse.x / pixelsize;
    *y = inpst.mouse.y / pixelsize;
}

void PMouseToPixels(int *x, int *y)
{
    *x = inpst.mouse.px / pixelsize;
    *y = inpst.mouse.py / pixelsize;
}

void MouseResetPrev()
{
    inpst.mouse.px = inpst.mouse.x;
    inpst.mouse.py = inpst.mouse.y;
}

void MouseToGrid(int *x, int *y)
{
    MouseToPixels(x, y);
    *x /= 8;
    *y /= 8;
}

void MouseToHex(Display_t* d, int *z, int *n)
{
    int a = d->angle;
    int det = hcos(a) * hsin(a + 8) - hsin(a) * hcos(a + 8);

    int x, y;
    MouseToPixels(&x, &y);

    PixelToHex(d, &x, &y);

    *z = x;
    *n = y;

    /*
    x += -d->screen.x - d->screen.w / 2 - d->screen_shift.x;
    y += -d->screen.y - d->screen.h / 2 - d->screen_shift.y;

    *z = hdiv( x * hsin(a + 8) - y * hcos(a + 8), det );
    *n = hdiv(-x * hsin(  a  ) + y * hcos(  a  ), det);
    */
    /*
    dz = hdiv( dx * hsin(a + 8) - dy * hcos(a + 8), det );
    dn = hdiv(-dx * hsin(  a  ) + dy * hcos(  a  ), det );

    *z += dz;
    *n += dn;
    */
}
