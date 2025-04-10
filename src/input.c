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

    inpst.left = 0;
    inpst.right = 0;
    inpst.up = 0;
    inpst.down = 0;
    inpst.vx = 0;
    inpst.vy = 0;
    inpst.shift = 0;
}

void InputRegister()
{
    inpst.mouse.down = 0;
    inpst.mouse.up   = 0;
    while( SDL_PollEvent( &event ) )
    {   
        switch( event.type ){
        case SDL_KEYDOWN:
            switch( event.key.keysym.sym ){
                case SDLK_LEFT:
                    inpst.left = 1;
                    break;
                case SDLK_RIGHT:
                    inpst.right = 1;
                    break;
                case SDLK_UP:
                    inpst.up = 1;
                    break;
                case SDLK_DOWN:
                    inpst.down = 1;
                    break;
                case SDLK_LSHIFT:
                    inpst.shift = 1;
                    break;
                case SDLK_x:
                    inpst.delete = 1;
                    break;
                case SDLK_c:
                    inpst.insertA = 1;
                    break;
                case SDLK_v:
                    inpst.insertB = 1;
                    break;
                case SDLK_SPACE:
                    inpst.jump = 1;
                    break;
                default:
                    break;
            }
            break;

        case SDL_KEYUP:
            switch( event.key.keysym.sym ){
                case SDLK_LEFT:
                    inpst.left = 0;
                    break;
                case SDLK_RIGHT:
                    inpst.right = 0;
                    break;
                case SDLK_UP:
                    inpst.up = 0;
                    break;
                case SDLK_DOWN:
                    inpst.down = 0;
                    break;
                case SDLK_LSHIFT:
                    inpst.shift = 0;
                    break;
                case SDLK_x:
                    inpst.delete = 0;
                    break;
                case SDLK_c:
                    inpst.insertA = 0;
                    break;
                case SDLK_v:
                    inpst.insertB = 0;
                    break;
                case SDLK_SPACE:
                    inpst.jump = 0;
                    break;
                default:
                    break;
            }
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
            inpst.mouse.scroll += event.wheel.y;
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

    int x, y, subz, subn;
    MouseToPixels(&x, &y);

    PixelToHex(d, &x, &y);

    *z = x;
    *n = y;
    
    subz = *z, subn = *n;
    
    if(d->scale <= -2)
    {
        *z = hdiv(*z, -d->scale);
        *n = hdiv(*n, -d->scale);
        int s = -d->scale, s1 = s, s2 = 2 * s;
        subz = mod( subz, s);
        subn = mod( subn, s);
        if(mod(s , 3) == 0) s1++, s2++;
        int conds[5] = {
            subz + 2 * subn < s1,
            2 * subz + subn <= s1,
            subz - subn < 0,
            subz + 2 * subn < s2,
            2 * subz + subn <= s2
        };
        printf("\n");
        if      (conds[0] == 1 && conds[1] == 1);
        else if (conds[2] == 0 && conds[3] == 1) *z = *z + 1;
        else if (conds[4] == 1) *n = *n + 1;
        else    *z = *z + 1, *n = *n + 1;
    }
    else if(d->scale >= 1)
    {
        *z = *z * d->scale;
        *n = *n * d->scale;
    }
}
