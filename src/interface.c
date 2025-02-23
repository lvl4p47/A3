#include "interface.h"

const int listsize = 2;

extern InputState_t inpst;

Button_t** buttonlist;

Cursor_t cursor;

int panning_b;

void ListInitialize()
{
    buttonlist = (Button_t**)malloc(listsize * sizeof(Button_t*));

	for (int i = 0; i < listsize; i++)
	{
		buttonlist[i] = NULL;
    }
    buttonlist[0] = ButtonInitialize(1, 1, 3, 3, 0, L"/=\\‖o‖\\=/");
    buttonlist[1] = ButtonInitialize(5, 1, 3, 3, 1, L"r-`___\\_№");

    cursor.lm = 1;
    cursor.rm = 2;
    cursor.lrad = 1;
    cursor.rrad = 4;

    panning_b = 0;
}

void ListTerminate()
{
    for(int i = 0; i < listsize; i++)
    {
        ButtonTerminate(buttonlist[i]);
    }
    free(buttonlist);
}

Button_t* ButtonInitialize(int x, int y, int w, int h, int type, wchar_t* s)
{
    Button_t* b = (Button_t*)malloc(sizeof(Button_t));
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
    b->string = s;
    b->type = type;
    return b;
}

void ButtonTerminate(Button_t* b)
{
    free(b);
}

void ButtonDraw(Button_t* b, Font_t* f)
{
    FontStringDraw(f, b->x, b->y, b->w, b->h, b->string);
}

void ListDraw(Font_t* f)
{
    for(int i = 0; i < listsize; i++)
    {
        ButtonDraw(buttonlist[i], f);
    }
    FontNumberDraw(f, 9, 1, 1, 1, cursor.lm);
    FontNumberDraw(f, 11, 1, 1, 1, cursor.rm);
}

void ListCheck()
{
    Button_t* b;
    int x, y;
    MouseToGrid(&x, &y);
    if(inpst.mouse.pressed)
    {
        for(int i = 0; i < listsize; i++)
        {
            b = buttonlist[i];
            if( isinrec(b->x, b->y, b->w, b->h, x, y) == 1)
            {
                ButtonAction(b);
            }
        }
    }
    else
    {
        for(int i = 0; i < listsize; i++)
        {
            buttonlist[i]->act_b = 0;
        }
    }
}

void ButtonAction(Button_t* b)
{
    if(b->act_b == 0)
    {
        switch (b->type)
        {
        case 0:
            if(inpst.mouse.lmc)
                cursor.lm = 1 + (cursor.lm) % 3;
            if(inpst.mouse.rmc)
                cursor.rm = 1 + (cursor.rm) % 3;
            b->act_b = 1;
            break;
        case 1:
            if(inpst.mouse.lmc)
                cursor.lm = 0;
            if(inpst.mouse.rmc)
                cursor.rm = 0;
            b->act_b = 1;
            break;
        }
        printf("\nbutton");
    }
}

void DisplayPanning(Kvad_t* ptr, Display_t* d)
{
    int wx, wy, pwx, pwy;
    int x, y, dz, dn, gridx, gridy;
    MouseToGrid(&gridx, &gridy);
    
    if(isinrec(17, 1, 43, 43, gridx, gridy) && inpst.mouse.down)
        panning_b = 1;

    if(inpst.mouse.pressed && panning_b)
    {
        if(inpst.mouse.mmc)
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
    }
    if(inpst.mouse.up)
    {
            d->screen_shift.w = d->screen_shift.x;
            d->screen_shift.h = d->screen_shift.y;
            panning_b = 0;
            printf( "UP\n");
    }
    if(inpst.mouse.wheel && (panning_b || isinrec(17, 1, 43, 43, gridx, gridy)))
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
    if(isinrec(17, 1, 43, 43, gridx, gridy) == 0)
    {
        d->screen_shift.w = d->screen_shift.x;
        d->screen_shift.h = d->screen_shift.y;
        MouseResetPrev();
    }
}

void ScreenInput(Kvad_t* ptr, Display_t* d)
{
    int hexx, hexy, gridx, gridy;
    MouseToGrid(&gridx, &gridy);

    if(inpst.mouse.pressed && isinrec(17, 1, 43, 43, gridx, gridy))
    {
        MouseToHex(d, &hexx, &hexy);

        if(inpst.mouse.lmc)
            KvadSetBlob(ptr, hexx, hexy, cursor.lm, cursor.lrad);
        if(inpst.mouse.rmc)
        {
            KvadSetBlob(ptr, hexx, hexy, cursor.rm, cursor.rrad);
        }
    }
}

void ButtonText(Button_t* b, wchar_t* s)
{
    b->string = s;
}

/*

ButtonDraw()

ButtonAction()

ButtonInitialize()

ButtonTerminate()

ButtonText()


*/