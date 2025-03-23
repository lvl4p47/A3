#include "interface.h"

const int buttonlistsize = 32;

const int sliderlistsize = 2;

const int inform_color = 7;
const int interact_color = 4;
const int frame_color = 3;

extern InputState_t inpst;

Button_t** buttonlist;

Slider_t** sliderlist;

Cursor_t cursor;

RulesEditor_t rules_editor;

int b_panning, b_pause, b_step, b_drawing, b_button, b_slider, b_grab;
int max_curs;
int b_ui;

uint64_t t_f;
uint64_t t_s;

//int mat_from, mat_to, cond_num;

void InterfaceInitialize()
{
    cursor.lm = 7;
    cursor.rm = 0;
    cursor.lrad = 1;
    cursor.rrad = 1;
    
    rules_editor.x = 1;
    rules_editor.y = 24;
    rules_editor.w = 15;
    rules_editor.h = 19;
    rules_editor.list_begin = 0;
    rules_editor.list_end = hmin(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->num, 3);
    rules_editor.mat_from    = 3;
    rules_editor.mat_to      = 6;
    rules_editor.cond_num    = 0;
    
    t_f = SDL_GetPerformanceFrequency() / 60;
    t_s = SDL_GetPerformanceFrequency() / 25;
    
    ButtonListInitialize();
    SliderListInitialize();

    b_panning   = 0;
    b_pause     = 0;
    b_step      = 0;
    b_ui        = 0;
    b_drawing   = 0;
    b_button    = -1;
    b_slider    = -1;
    b_grab      = 0;
    
    max_curs    = 8;
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
    buttonlist[0] = ButtonInitialize(1  , 1 , 3, 3, 0, L"/=\\‖ ‖\\=/", L".../=\\‖ ‖"); //L"/=\\‖o‖\\=/", L".../=\\‖o‖"
    buttonlist[1] = ButtonInitialize(5  , 1 , 3, 3, 1, L"r-`___\\_№", L"...r-`___");
    buttonlist[2] = ButtonInitialize(61 , 20, 3, 3, 2, L"n n‖ ‖u u", L"|\\ | >|/ ");
    buttonlist[3] = ButtonInitialize(1  , 5 , 3, 3, 3, L" ‖‖nVV\\l№", L"   /mm\\l№");
    buttonlist[4] = ButtonInitialize(66 , 20, 3, 3, 4, L"\\ | >|/ |", L" \\   > / ");
    buttonlist[5] = ButtonInitialize(71 , 20, 3, 3, 5, L" _ <o> - ", L"\\_/<X>/-\\");
    buttonlist[6] = ButtonInitialize(79 , 0 , 1, 1, 6, L"X", L"+");
    buttonlist[7] = ButtonInitialize(9  , 1 , 3, 3, 7, L" T lв№\\_/", L".(.rT`lв№");
    buttonlist[8] = ButtonInitialize(78 , 0 , 1, 1, 8, L"o", L"0");
    buttonlist[9] = ButtonInitialize(77 , 0 , 1, 1, 9, L"-", L"_");
    
    buttonlist[10] = ButtonInitialize(rules_editor.x + 13, rules_editor.y + 0, 2, 1, 10, L"/\\", L"№l");
    buttonlist[11] = ButtonInitialize(rules_editor.x + 13, rules_editor.y + 3, 2, 1, 11, L"/\\", L"№l");
    buttonlist[12] = ButtonInitialize(rules_editor.x + 13, rules_editor.y + 6, 2, 1, 12, L"/\\", L"№l");
    buttonlist[13] = ButtonInitialize(rules_editor.x + 13, rules_editor.y + 2, 2, 1, 13, L"\\/", L"`r");
    buttonlist[14] = ButtonInitialize(rules_editor.x + 13, rules_editor.y + 5, 2, 1, 14, L"\\/", L"`r");
    buttonlist[15] = ButtonInitialize(rules_editor.x + 13, rules_editor.y + 8, 2, 1, 15, L"\\/", L"`r");
    
    buttonlist[16] = ButtonInitialize(rules_editor.x + 2, rules_editor.y + 0, 2, 1, 16, L"/\\", L"№l");
    buttonlist[17] = ButtonInitialize(rules_editor.x + 2, rules_editor.y + 2, 2, 1, 17, L"\\/", L"`r");
    buttonlist[18] = ButtonInitialize(rules_editor.x + 6, rules_editor.y + 0, 2, 1, 18, L"/\\", L"№l");
    buttonlist[19] = ButtonInitialize(rules_editor.x + 6, rules_editor.y + 2, 2, 1, 19, L"\\/", L"`r");
    buttonlist[20] = ButtonInitialize(rules_editor.x + 8, rules_editor.y + 3, 2, 1, 20, L"/\\", L"№l");
    buttonlist[21] = ButtonInitialize(rules_editor.x + 8, rules_editor.y + 5, 2, 1, 21, L"\\/", L"`r");
    buttonlist[22] = ButtonInitialize(rules_editor.x + 6, rules_editor.y + 6, 2, 1, 22, L"/\\", L"№l");
    buttonlist[23] = ButtonInitialize(rules_editor.x + 6, rules_editor.y + 8, 2, 1, 23, L"\\/", L"`r");
    buttonlist[24] = ButtonInitialize(rules_editor.x + 2, rules_editor.y + 6, 2, 1, 24, L"/\\", L"№l");
    buttonlist[25] = ButtonInitialize(rules_editor.x + 2, rules_editor.y + 8, 2, 1, 25, L"\\/", L"`r");
    buttonlist[26] = ButtonInitialize(rules_editor.x + 0, rules_editor.y + 3, 2, 1, 26, L"/\\", L"№l");
    buttonlist[27] = ButtonInitialize(rules_editor.x + 0, rules_editor.y + 5, 2, 1, 27, L"\\/", L"`r");
    
    buttonlist[28] = ButtonInitialize(rules_editor.x + 4, rules_editor.y + 3, 2, 1, 28, L"/\\", L"№l");
    buttonlist[29] = ButtonInitialize(rules_editor.x + 4, rules_editor.y + 5, 2, 1, 29, L"\\/", L"`r");
    
    buttonlist[30] = ButtonInitialize(rules_editor.x + 11, rules_editor.y + 6, 1, 1, 30, L"+", L"*");
    buttonlist[31] = ButtonInitialize(rules_editor.x + 11, rules_editor.y + 8, 1, 1, 31, L"-", L".");
    
    //buttonlist[] = ButtonInitialize(, , , , , L"", L"");
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
            b_grab = 1 - b_grab;
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
        case 7:
            swap(&cursor.rm, &cursor.lm);
            b->act_b = 1;
            b->text = 3 - b->text;
            break;
        case 8:
            ResizeWindow(5 - pixelsize);
            b->act_b = 1;
            b->text = 3 - b->text;
            break;
        case 9:
            ToggleWindow(1);
            b->act_b = 1;
            b->text = 2;
            break;
            
        case 10:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->flag = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->flag + 2, 2) - 1 ;
            b->act_b = 1;
            b->text = 2;
            break;
        case 11:
            rules_editor.mat_to = 
            mod(rules_editor.mat_to + 1, mat_amount);
            rules_editor.cond_num = 0;
            b->act_b = 1;
            b->text = 2;
            break;
        case 12:
            rules_editor.cond_num =
            mod(rules_editor.cond_num + 1, RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->num) ;
            b->act_b = 1;
            b->text = 2;
            break;
        case 13:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->flag = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->flag + 2, 2) - 1 ;
            b->act_b = 1;
            b->text = 2;
            break;
        case 14:
            rules_editor.mat_to = 
            mod(rules_editor.mat_to - 1, mat_amount);
            rules_editor.cond_num = 0;
            b->act_b = 1;
            b->text = 2;
            break;
        case 15:
            rules_editor.cond_num =
            mod(rules_editor.cond_num - 1, RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->num) ;
            b->act_b = 1;
            b->text = 2;
            break;
            
        case 16:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[0] = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[0] + 2, mat_amount + 1) - 1;
            b->act_b = 1;
            b->text = 2;
            break;
        case 17:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[0] = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[0] + 0, mat_amount + 1) - 1;
            b->act_b = 1;
            b->text = 2;
            break;
            
        case 18:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[1] = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[1] + 2, mat_amount + 1) - 1;
            b->act_b = 1;
            b->text = 2;
            break;
        case 19:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[1] = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[1] + 0, mat_amount + 1) - 1;
            b->act_b = 1;
            b->text = 2;
            break;
            
        case 20:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[2] = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[2] + 2, mat_amount + 1) - 1;
            b->act_b = 1;
            b->text = 2;
            break;
        case 21:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[2] = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[2] + 0, mat_amount + 1) - 1;
            b->act_b = 1;
            b->text = 2;
            break;
        
        case 22:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[3] = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[3] + 2, mat_amount + 1) - 1;
            b->act_b = 1;
            b->text = 2;
            break;
        case 23:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[3] = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[3] + 0, mat_amount + 1) - 1;
            b->act_b = 1;
            b->text = 2;
            break;
        
        case 24:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[4] = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[4] + 2, mat_amount + 1) - 1;
            b->act_b = 1;
            b->text = 2;
            break;
        case 25:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[4] = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[4] + 0, mat_amount + 1) - 1;
            b->act_b = 1;
            b->text = 2;
            break;
            
        case 26:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[5] = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[5] + 2, mat_amount + 1) - 1;
            b->act_b = 1;
            b->text = 2;
            break;
        case 27:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[5] = 
            mod(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[5] + 0, mat_amount + 1) - 1;
            b->act_b = 1;
            b->text = 2;
            break;
            
        case 28:
            rules_editor.mat_from = 
            mod(rules_editor.mat_from + 1, mat_amount);
            rules_editor.cond_num = 0;
            b->act_b = 1;
            b->text = 2;
            break;
        case 29:
            rules_editor.mat_from = 
            mod(rules_editor.mat_from - 1, mat_amount);
            rules_editor.cond_num = 0;
            b->act_b = 1;
            b->text = 2;
            break;
            
        case 30:
            RulesAdd(rules_editor.mat_from, rules_editor.mat_to, -1, -1, -1, -1, -1, -1, -1);
            rules_editor.cond_num = RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->num - 1;
            b->act_b = 1;
            b->text = 2;
            break;
        case 31:
            RulesRemove(rules_editor.mat_from, rules_editor.mat_to, rules_editor.cond_num);
            rules_editor.cond_num = RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->num - 1;
            if(rules_editor.list_begin > 0) rules_editor.list_begin--;
            b->act_b = 1;
            b->text = 2;
            break;
            
        default:
            b->act_b = 1;
            b->text = 2;
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
        case 8:
            break;
        case 9:
            b->text = 1;
            break;
        default:
            b->text = 1;
            break;
        }
    ButtonText(b, b->text);
}

void DisplayPanning(Kvad_t* ptr, Display_t* d)
{
    int wx, wy, pwx, pwy;
    int x, y, dz, dn, gridx, gridy;
    MouseToGrid(&gridx, &gridy);
    
    if(isinrec(17, 1, 43, 43, gridx, gridy) && inpst.mouse.down 
    && ((inpst.mouse.mmc == 1 && b_grab == 0) ||
        (inpst.mouse.lmc == 1 && b_grab == 1)))
        b_panning = 1, MouseResetPrev();;
    if(((inpst.mouse.mmc == 1 && b_grab == 0) ||
        (inpst.mouse.lmc == 1 && b_grab == 1)) && b_panning)
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
    if((inpst.mouse.mmc == 0 && b_grab == 0) ||
        (inpst.mouse.lmc == 0 && b_grab == 1))
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
    
    if((inpst.mouse.scroll)
         && (b_panning || isinrec(17, 1, 43, 43, gridx, gridy)))
    {
        d->angle = (d->angle + 48 + inpst.mouse.scroll) % 48;
        inpst.mouse.scroll = 0;

        d->screen_shift.x = 1 * d->hshift.w +
        (d->hshift.x) * hcos(d->angle) + (d->hshift.y) * hcos(d->angle + 8);
        d->screen_shift.y = 1 * d->hshift.h +
        (d->hshift.x) * hsin(d->angle) + (d->hshift.y) * hsin(d->angle + 8);

        inpst.mouse.px = inpst.mouse.x;
        inpst.mouse.py = inpst.mouse.y;

        d->screen_shift.w = d->screen_shift.x;
        d->screen_shift.h = d->screen_shift.y;

    }
    else
    {
        inpst.mouse.scroll = 0;
    }
}

void ScreenInput(Kvad_t* ptr, Display_t* d)
{
    int hexx, hexy, gridx, gridy;
    MouseToGrid(&gridx, &gridy);
    
    if(isinrec(17, 1, 43, 43, gridx, gridy) && inpst.mouse.down && b_grab == 0)
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
    sliderlist[0] = SliderInitialize(1, 9, 11, 3, 0, 1, c0, 0);
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
    FontStringDraw(f, s->x + s->c - 1, s->y, 3, 2, L" V с=э", interact_color);
    int value = s->a + s->d * (s->c - 1);
    FontNumberDraw(f, s->x + s->c - 1, s->y + 2, 3, 1, value, interact_color);
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
    
    FontStringDraw(f, 1, 18, 15, 16,
    str, inform_color);
    /*
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
    if(b_ui)
    {
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
    
    FontRulesEditorDraw(f);
    
    
}

void FontRulesEditorDraw(Font_t* f)
{
    int rules_x = rules_editor.x, rules_y = rules_editor.y;
    
    FontNumberDraw(f, rules_x + 4, rules_y + 4,     2, 1, rules_editor.mat_from, inform_color);
    
    FontNumberDraw(f, rules_x + 13  , rules_y + 1   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->flag, inform_color);
    FontNumberDraw(f, rules_x + 13  , rules_y + 4   , 2, 1, rules_editor.mat_to, inform_color);
    FontNumberDraw(f, rules_x + 13  , rules_y + 7   , 2, 1, rules_editor.cond_num, inform_color);
            
    FontNumberDraw(f, rules_x + 2   , rules_y + 1   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[0], inform_color);
    FontNumberDraw(f, rules_x + 6   , rules_y + 1   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[1], inform_color);
    FontNumberDraw(f, rules_x + 8   , rules_y + 4   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[2], inform_color);
    FontNumberDraw(f, rules_x + 6   , rules_y + 7   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[3], inform_color);
    FontNumberDraw(f, rules_x + 2   , rules_y + 7   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[4], inform_color);
    FontNumberDraw(f, rules_x + 0   , rules_y + 4   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[5], inform_color);
    
    FontCharDraw(f, L'P', rules_x + 11, rules_y + 1, inform_color);
    FontCharDraw(f, L'>', rules_x + 11, rules_y + 4, inform_color);
    FontCharDraw(f, L'‖', rules_x + 11, rules_y + 7, inform_color);
    
    int tree_x = 1, tree_y = 12, list_y = 0, list_h = hmax(rules_editor.h - tree_y + 1, 1);
    
    list_y = hmax(rules_editor.cond_num - list_h, 0);
    
    FontNumberDraw(f, rules_x + tree_x , rules_y + tree_y - 2 , 2, 1, 
            rules_editor.mat_from, inform_color);
            
    FontStringDraw(f, rules_x + tree_x + 3 , rules_y + tree_y - 2, 2, 1, L"->", inform_color);
    
    FontNumberDraw(f, rules_x + tree_x + 6 , rules_y + tree_y - 2 , 2, 1, 
            rules_editor.mat_to, inform_color);
            
    if(rules_editor.cond_num >= rules_editor.list_begin + list_h) rules_editor.list_begin++;
    if(rules_editor.cond_num < rules_editor.list_begin) rules_editor.list_begin--;
    
    rules_editor.list_end = hmin(rules_editor.list_begin + list_h,
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->num);
            
    int arrow_y = 
        hmax(
            hmin( rules_editor.y + rules_editor.h, rules_y + tree_y + 
                rules_editor.cond_num - rules_editor.list_begin),
            rules_editor.y
        );
    FontCharDraw(f, L'>', 
            hmin( rules_editor.x + rules_editor.w, rules_x + tree_x - 1 ), 
            arrow_y, inform_color);
    
    for(int number = rules_editor.list_begin; number < rules_editor.list_end; number++)
    {
        FontNumberDraw(f, 
            rules_x + tree_x, 
            rules_y + tree_y + number  - rules_editor.list_begin, 2, 1, 
            RULES->frommat[rules_editor.mat_from]->
            tomat[rules_editor.mat_to]->
            req[number]->flag, inform_color);
        for(int k = 0; k < 6; k++)
        {
            FontNumberDraw(f, 
                rules_x + tree_x + 2 + k * 2 , 
                rules_y + tree_y + number - rules_editor.list_begin, 2, 1, 
                RULES->frommat[rules_editor.mat_from]->
                tomat[rules_editor.mat_to]->
                req[number]->neighbors[k], inform_color);
        }
        
    }
    
    //hmin( rules_editor.x + rules_editor.w, 
    //hmin( rules_editor.y + rules_editor.h, 
}