#include "interface.h"

const int buttonlistsize = 7;

const int sliderlistsize = 2;

const int inform_color = 2;
const int interact_color = 7;
const int frame_color = 3;

extern InputState_t inpst;

Button_t** buttonlist;

Slider_t** sliderlist;

Cursor_t cursor;

int b_panning, b_pause, b_step, b_drawing, b_button, b_slider;
int max_curs;
int b_ui;

uint64_t t_f;
uint64_t t_s;

void InterfaceInitialize()
{
    cursor.lm = 3;
    cursor.rm = 0;
    cursor.lrad = 1;
    cursor.rrad = 1;
    
    t_f = SDL_GetPerformanceFrequency() / 60;
    t_s = SDL_GetPerformanceFrequency() / 25;
    
    ButtonListInitialize();
    SliderListInitialize();

    b_panning = 0;
    b_pause = 0;
    b_step = 0;
    b_ui = 0;
    b_drawing = 0;
    b_button = -1;
    b_slider = -1;
    
    max_curs = 8;
    // t_s = (sliderlist[1]->a + sliderlist[1]->d * (sliderlist[1]->c - 1))
    //      * SDL_GetPerformanceFrequency() / 1000;
}

void InterfaceTerminate()
{
    ButtonListTerminate();
}

void InterfaceDraw(Font_t* f, Display_t* d)
{
    FontUIDraw(f, d);
    ButtonListDraw(f);
    SliderListDraw(f);
}

void InterfaceUpdate()
{
    ButtonListCheck();
    SliderListCheck();
}

void ButtonListInitialize()
{
    buttonlist = (Button_t**)malloc(buttonlistsize * sizeof(Button_t*));

	for (int i = 0; i < buttonlistsize; i++)
	{
		buttonlist[i] = NULL;
    }
    buttonlist[0] = ButtonInitialize(1, 1, 3, 3, 0, L"/=\\‖ ‖\\=/", L".../=\\‖ ‖"); //L"/=\\‖o‖\\=/", L".../=\\‖o‖"
    buttonlist[1] = ButtonInitialize(5, 1, 3, 3, 1, L"r-`___\\_№", L"...r-`___");
    buttonlist[2] = ButtonInitialize(61, 20, 3, 3, 2, L"n n‖ ‖u u", L"|\\ | >|/ ");
    buttonlist[3] = ButtonInitialize(66, 20, 3, 3, 3, L"ъеъеъеъеъ", L"еъеъеъеъе");
    buttonlist[4] = ButtonInitialize(71, 20, 3, 3, 4, L"\\ | >|/ |", L" \\   > / ");
    buttonlist[5] = ButtonInitialize(76, 20, 3, 3, 5, L" _ <o> - ", L"\\_/<X>/-\\");
    buttonlist[6] = ButtonInitialize(79, 0, 1, 1, 6, L"X", L"+");
}

void ButtonListTerminate()
{
    for(int i = 0; i < buttonlistsize; i++)
    {
        ButtonTerminate(buttonlist[i]);
    }
    free(buttonlist);
}

Button_t* ButtonInitialize(int x, int y, int w, int h, int type, wchar_t* s, wchar_t* ss)
{
    Button_t* b = (Button_t*)malloc(sizeof(Button_t));
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
    b->string = s;
    b->sscndr = ss;
    b->type = type;
    b->text = 1;
    b->b_grab = 0;
    return b;
}

void ButtonTerminate(Button_t* b)
{
    free(b);
}

void ButtonDraw(Button_t* b, Font_t* f)
{
    if(b->text == 1)
        FontStringDraw(f, b->x, b->y, b->w, b->h, b->string, interact_color);
    else
        FontStringDraw(f, b->x, b->y, b->w, b->h, b->sscndr, interact_color);
}

void ButtonListDraw(Font_t* f)
{
    for(int i = 0; i < buttonlistsize; i++)
    {
        ButtonDraw(buttonlist[i], f);
    }
    FontNumberDraw(f, 9, 1, 1, 1, cursor.lm, interact_color);
    FontNumberDraw(f, 11, 1, 1, 1, cursor.rm, interact_color);
}

void ButtonListCheck()
{
    Button_t* b;
    int x, y;
    MouseToGrid(&x, &y);
    if(inpst.mouse.pressed && b_panning == 0 && b_drawing == 0)
    {
        for(int i = 0; i < buttonlistsize; i++)
        {
            b = buttonlist[i];
            if( (isinrec(b->x, b->y, b->w, b->h, x, y) == 1
                 && b_slider == -1 && (b_button == i || b_button == -1))
                 || (b->b_grab == 1 && b_button == i) )
            {
                ButtonDown(b);
                b->b_grab = 1;
                b_button = i;
            }
            else
            {
                ButtonUp(b);
                b->b_grab = 0;
            }
        }
    }
    else
    {
        for(int i = 0; i < buttonlistsize; i++)
        {
            b = buttonlist[i];
            ButtonUp(buttonlist[i]);
            b->b_grab = 0;
            b_button = -1;
        }
    }
}

void ButtonDown(Button_t* b)
{
    if(b->act_b == 0)
    {
        switch (b->type)
        {
        case 0:
            if(inpst.mouse.lmc)
                cursor.lm = 1 + (cursor.lm) % max_curs;
            if(inpst.mouse.rmc)
                cursor.rm = 1 + (cursor.rm) % max_curs;
            b->act_b = 1;
            b->text = 2;
            break;
        case 1:
            if(inpst.mouse.lmc)
                cursor.lm = 0;
            if(inpst.mouse.rmc)
                cursor.rm = 0;
            b->act_b = 1;
            b->text = 2;
            break;
        case 2:
            b_pause = 1 - b_pause;
            b->act_b = 1;
            b->text = 3 - b->text;
            break;
        case 3:
            b->act_b = 1;
            b->text = 3 - b->text;
            break;
        case 4:
            b_step = 1;
            b->act_b = 1;
            b->text = 2;
            break;
        case 5:
            b_ui = 1 - b_ui;
            b->act_b = 1;
            b->text = 3 - b->text;
            break;
        case 6:
            b_quit = 1;
            b->act_b = 1;
            b->text = 3 - b->text;
            break;
        default:
            break;
        }
    }
}

void ButtonUp(Button_t* b)
{
    b->act_b = 0;
    switch (b->type)
        {
        case 0:
            b->text = 1;
            break;
        case 1:
            b->text = 1;
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            b->text = 1;
            break;
        case 5:
            break;
        }
    ButtonText(b, b->text);
}

void DisplayPanning(Kvad_t* ptr, Display_t* d)
{
    int wx, wy, pwx, pwy;
    int x, y, dz, dn, gridx, gridy;
    MouseToGrid(&gridx, &gridy);
    
    if(isinrec(17, 1, 43, 43, gridx, gridy) && inpst.mouse.down && inpst.mouse.mmc)
        b_panning = 1;
    if(inpst.mouse.mmc && b_panning)
    {
        MouseToPixels(&wx, &wy);
        PMouseToPixels(&pwx, &pwy);
        d->screen_shift.x = d->screen_shift.w + wx - pwx;
        d->screen_shift.y = d->screen_shift.h + wy - pwy;
        
        d->hshift.x = 4 + d->screen.x + d->screen_shift.x * 2  + d->screen.w / 2;
        d->hshift.y = 4 + d->screen.y + d->screen_shift.y * 2  + d->screen.h / 2;
        PixelToHex(d, &d->hshift.x, &d->hshift.y);
        
        
        d->hshift.w = d->screen_shift.x -
        (d->hshift.x) * hcos(d->angle) - (d->hshift.y) * hcos(d->angle + 8);
        d->hshift.h = d->screen_shift.y -
        (d->hshift.x) * hsin(d->angle) - (d->hshift.y) * hsin(d->angle + 8);
        
    }
    if(inpst.mouse.mmc == 0)
    {
        d->screen_shift.w = d->screen_shift.x;
        d->screen_shift.h = d->screen_shift.y;
    
        d->hshift.x = 4 + d->screen.x + d->screen_shift.x * 2  + d->screen.w / 2;
        d->hshift.y = 4 + d->screen.y + d->screen_shift.y * 2  + d->screen.h / 2;
        PixelToHex(d, &d->hshift.x, &d->hshift.y);
            
        d->hshift.w = d->screen_shift.x -
        (d->hshift.x) * hcos(d->angle) - (d->hshift.y) * hcos(d->angle + 8);
        d->hshift.h = d->screen_shift.y -
        (d->hshift.x) * hsin(d->angle) - (d->hshift.y) * hsin(d->angle + 8);
            
        MouseResetPrev();
        b_panning = 0;
    }
    
    if(inpst.mouse.wheel && (b_panning || isinrec(17, 1, 43, 43, gridx, gridy)))
    {
        d->angle = (d->angle + 48 + inpst.mouse.scroll) % 48;
        inpst.mouse.wheel = 0;

        d->screen_shift.x = 1 * d->hshift.w +
        (d->hshift.x) * hcos(d->angle) + (d->hshift.y) * hcos(d->angle + 8);
        d->screen_shift.y = 1 * d->hshift.h +
        (d->hshift.x) * hsin(d->angle) + (d->hshift.y) * hsin(d->angle + 8);

        inpst.mouse.px = inpst.mouse.x;
        inpst.mouse.py = inpst.mouse.y;

        d->screen_shift.w = d->screen_shift.x;
        d->screen_shift.h = d->screen_shift.y;

    }
    
    /*
    if(isinrec(17, 1, 43, 43, gridx, gridy) == 0 )
    {
        d->screen_shift.w = d->screen_shift.x;
        d->screen_shift.h = d->screen_shift.y;
        MouseResetPrev();
    }
    */
}

void ScreenInput(Kvad_t* ptr, Display_t* d)
{
    int hexx, hexy, gridx, gridy;
    MouseToGrid(&gridx, &gridy);
    
    if(isinrec(17, 1, 43, 43, gridx, gridy) && inpst.mouse.down)
        b_drawing = 1;

    if(inpst.mouse.pressed && isinrec(17, 1, 43, 43, gridx, gridy) && b_drawing)
    {
        MouseToHex(d, &hexx, &hexy);

        if(inpst.mouse.lmc)
            KvadSetBlob(ptr, hexx, hexy, cursor.lm, cursor.lrad);
        if(inpst.mouse.rmc)
        {
            KvadSetBlob(ptr, hexx, hexy, cursor.rm, cursor.rrad);
            //KvadGetHexel(ptr, hexx, hexy)->dx = 1;
        }
    }
    if(inpst.mouse.up)
    {
            b_drawing = 0;
    }
}

void ButtonText(Button_t* b, int n)
{
    b->text = n;
}

Slider_t* SliderInitialize(int x, int y, int w, int h, int a, int d, int c, int type)
{
    Slider_t* s = (Slider_t*)malloc(sizeof(Slider_t));
    s->x = x;
    s->y = y;
    s->w = hmax(w, 3);
    s->h = hmax(h, 3);
    s->a = a;
    s->d = d;
    s->c = c;
    s->type = type;
    s->b_grab = 0;
    return s;
}

void SliderTerminate(Slider_t* s)
{
    free(s);
}

void SliderListInitialize()
{
    sliderlist = (Slider_t**)malloc(sliderlistsize * sizeof(Slider_t*));

	for (int i = 0; i < sliderlistsize; i++)
	{
		sliderlist[i] = NULL;
    }
    int c0, c1;
    c0 = 1 + (cursor.lrad - 0) / 1;
    c1 = 1 + (t_s - 0) * 1000 / (20 * SDL_GetPerformanceFrequency());
    sliderlist[0] = SliderInitialize(1, 5, 11, 3, 0, 1, c0, 0);
    sliderlist[1] = SliderInitialize(61, 24, 18, 3, 0, 20, c1, 1);
}

void SliderListTerminate()
{
    for(int i = 0; i < sliderlistsize; i++)
    {
        SliderTerminate(sliderlist[i]);
    }
    free(sliderlist);
}

void SliderDraw(Slider_t* s, Font_t* f)
{
    FontCharFill(f, L'-', s->x + 1, s->y + 1, s->w - 2, 1, interact_color);
    FontStringDraw(f, s->x + s->c, s->y, 1, 2, L"V+", interact_color);
    int value = s->a + s->d * (s->c - 1);
    int raz = 0;
    int dva = 100;
    while(value >= dva)
    {
        raz++;
        dva = dva * 100;
    }
    
    FontNumberDraw(f, s->x + s->c - raz, s->y + 2, 3, 1, value, interact_color);
}

void SliderListDraw(Font_t* f)
{
    for(int i = 0; i < sliderlistsize; i++)
    {
        SliderDraw(sliderlist[i], f);
    }
}

void SliderListCheck()
{
    
    Slider_t* s;
    int x, y, c;
    MouseToGrid(&x, &y);
    if(inpst.mouse.pressed && b_panning == 0 && b_drawing == 0)
    {
        for(int i = 0; i < sliderlistsize; i++)
        {
            s = sliderlist[i];
            if( (isinrec(s->x, s->y, s->w, s->h, x, y) == 1
                 && b_button == -1 && (b_slider == i || b_slider == -1))
                 || (s->b_grab == 1 && b_slider == i) )
            {
                c = hmax( 1, hmin((x - s->x), s->w - 2));
                SliderDown(s, c);
                s->b_grab = 1;
                b_slider = i;
            }
            else
            {
                SliderUp(s);
                s->b_grab = 0;
            }
        }
    }
    else
    {
        for(int i = 0; i < sliderlistsize; i++)
        {
            s = sliderlist[i];
            SliderUp(s);
            s->b_grab = 0;
            b_slider = -1;
        }
    }
    
}

void SliderDown(Slider_t* s, int c)
{
    switch (s->type)
    {
    case 0:
        s->c = c;
        cursor.lrad = s->a + s->d * (s->c - 1);
        cursor.rrad = s->a + s->d * (s->c - 1);
        break;
    case 1:
        s->c = c;
        t_s = (s->a + s->d * (s->c - 1)) * SDL_GetPerformanceFrequency() / 1000;
        break;
    default:
        break;
    }
}

void SliderUp(Slider_t* s)
{
    
}

void FontUIDraw(Font_t* f, Display_t* d)
{
    wchar_t reading[82];

    FILE* fl1 = NULL;
    fl1 = fopen("./src/ui.txt", "r");
    for(int i = 0; i < 45; i++)
    {
        fgetws(reading, 82, fl1);
        FontStringDraw(f, 0, i, 80, 1, reading, frame_color); 
    }
    fclose(fl1);

    int x, y;
    MouseToGrid(&x, &y);
    wchar_t str[240];
    swprintf(str, 240, L"угол: %i\nсдвиг по х: %i\nсдвиг по y: %i\nмышь х: %i\nмышь y: %i\n", 
    d->angle, d->hshift.x, d->hshift.y, x, y);
    
    FontStringDraw(f, 1, 28, 15, 16,
    str, inform_color);
    /*
    FontStringDraw(f, 1, 1, 3, 3, L"/=\\‖o‖\\=/");
    FontStringDraw(f, 5, 1, 3, 3, L"r `   l №");
    FontStringDraw(f, 9, 1, 3, 3, L"o\\ \\ \\ \\o");
    FontStringDraw(f, 1, 5, 3, 3, L" n <ш>iV ");

    FontStringDraw(f, 1, 10, 3, 3, L":::r`/`№ъ");
    FontStringDraw(f, 5, 10, 3, 3, L"n n>>>u u");

    FontStringDraw(f, 1, 18, 3, 4, L"c=\\ // u  o ");
    FontStringDraw(f, 1, 23, 3, 4, L"r\\ |l\\| |l-№");r\ 
    FontStringDraw(f, 5, 23, 3, 4, L" Л /‖\\ ‖ c=э"); |l\
    FontStringDraw(f, 9, 23, 3, 4, L" ‖ \\‖/ V c=э"); | |
    FontStringDraw(f, 13, 23, 3, 4, L" n qhpdчb u "); l-№
    */
    int r = 16;
    int ang = d->angle;

    int zang, nang;

    zang = r, nang = 0;   

    HexToGrid(d, &zang, &nang);
    FontNumberDraw(f, zang, nang, 3, 1, 0, inform_color);

    zang = r, nang = -r;
    HexToGrid(d, &zang, &nang);
    FontNumberDraw(f, zang, nang, 3, 1, 90, inform_color);

    zang = 0, nang = -r;
    HexToGrid(d, &zang, &nang);
    FontNumberDraw(f, zang, nang, 3, 1, 120, inform_color);

    zang = -r, nang = 0;
    HexToGrid(d, &zang, &nang);
    FontNumberDraw(f, zang, nang, 3, 1, 180, inform_color);

    zang = -r, nang = r;
    HexToGrid(d, &zang, &nang);
    FontNumberDraw(f, zang, nang, 3, 1, 240, inform_color);

    zang = 0, nang = r;
    HexToGrid(d, &zang, &nang);
    FontNumberDraw(f, zang, nang, 3, 1, 300, inform_color);
    
}