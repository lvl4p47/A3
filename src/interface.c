#include "interface.h"

const int buttonlistsize = 33;

const int sliderlistsize = 2;

const int inform_color = 7;
const int important_color = 5;
const int yes_color = 2;
const int no_color = 1;
const int maybe_color = 6;
const int interact_color = 4;
const int frame_color = 3;

extern InputState_t inpst;

Button_t** buttonlist;

Slider_t** sliderlist;

Cursor_t cursor;

RulesEditor_t rules_editor;

Select_List_t select_list;

ui_rectangle_t hex_screen, toolpad, control_panel;

InfoBox_t info_box;

wchar_t *s_rules_editor, *s_toolpad, *s_control_panel;

int b_panning, b_pause, b_step, b_drawing, b_button, b_slider, b_grab, b_select_list;
int max_curs;
int b_ui;

uint64_t t_f;
uint64_t t_s;

int min_neigh, max_neigh;

//int mat_from, mat_to, cond_num;

void InterfaceInitialize()
{
    cursor.lm = 5;
    cursor.rm = 0;
    cursor.lrad = 3;
    cursor.rrad = 3;
    
    rules_editor.x = 1;
    rules_editor.y = 24;
    rules_editor.w = 17;
    rules_editor.h = 20;
    rules_editor.list_begin = 0;
    rules_editor.list_end = hmin(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->num, 3);
    rules_editor.mat_from    = 2;
    rules_editor.mat_to      = 0;
    rules_editor.cond_num    = 0;
    
    select_list.rectangle.x = 1;
    select_list.rectangle.y = 1;
    select_list.rectangle.w = 17;
    select_list.rectangle.h = 6;
    select_list.s = L"0 - воздух\n1 - верёвка\n2 - огонь\n3 - вода\n4 - песок\n5 - земля\n6 - пар\n7 - лёд\n8 - камень";
    select_list.min = 0;
    select_list.max = 8;
    
    hex_screen.x = 19;
    hex_screen.y = 1;
    hex_screen.w = 43;
    hex_screen.h = 43;
    
    toolpad.x = 1;
    toolpad.y = 8;
    toolpad.w = 17;
    toolpad.h = 7;
    
    control_panel.x = 1;
    control_panel.y = 16;
    control_panel.w = 16;
    control_panel.h = 7;
    
    s_rules_editor = L"  Порядок установления значений:\n1) \"ИЗ\";\n2) \"В\";\n3) \"УСЛ\";\n4) остальное.\n\n  ФЛАГ:\n\"--\" игнорировать данный набор условий;\n\"+0\" необходимо >= зеленых соседей и < красных;\n\"+1\" необходимо точное (не)совпадение количества соседей.\n\n  Для перехода клетки из начального состояния в конечное необходимо выполнение хотя бы одного из условий в списке.";
    s_toolpad = L"  Правой / левой кнопкой мыши по списку можно задать материал курсора. Колесико мыши позволяет перемещаться по списку\n\n  Кнопки слева направо:\n* Поменять правое и левое значения курсора местами;\n* Переход в режим передвижения по массиву клеток с помощью ЛКМ.\n\n  Слайдер изменяет размер закрашивания курсором.";
    s_control_panel = L"  Кнопки слева направо:\n* Пауза;\n* Сделать 1 шаг клеточного автомата (во время паузы);\n* Переключить режим отображения плотности материалов;\n* Сместить выбираемые миникартой клетки.\n\n  Слайдер изменяет время шага клеточного автомата.";
       
    info_box.x = 63;
    info_box.y = 24;
    info_box.w = 16;
    info_box.h = 20;
    info_box.s = L"";
    info_box.shift = 0;
    
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
    b_select_list = 0;
    
    
    max_curs    = 8;
    
    min_neigh = -mat_amount - 1;
    max_neigh = mat_amount - 1;
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
    FontPrintToInfoBox(f);
    SelectListDraw(f, d, &select_list);
    inpst.mouse.scroll = 0;
}

void InterfaceUpdate()
{
    ButtonListCheck();
    SliderListCheck();
    InfoBoxUpdate();
    SelectListUpdate();
}

void ButtonListInitialize()
{
    buttonlist = (Button_t**)malloc(buttonlistsize * sizeof(Button_t*));

	for (int i = 0; i < buttonlistsize; i++)
	{
		buttonlist[i] = NULL;
    }
    buttonlist[0] = ButtonInitialize(toolpad.x + 9  , toolpad.y , 3, 3, 0, L".........", L"---------");
    buttonlist[1] = ButtonInitialize(toolpad.x + 13 , toolpad.y , 3, 3, 1, L".........", L"---------");
    buttonlist[2] = ButtonInitialize(control_panel.x + 1    , control_panel.y, 3, 3, 2, L"П П‖ ‖U U", L"|\\ | >|/ ");
    buttonlist[3] = ButtonInitialize(toolpad.x + 5  , toolpad.y , 3, 3, 3, L" ‖‖Лvv\\L№", L"   /mm\\L№");
    buttonlist[4] = ButtonInitialize(control_panel.x + 5    , control_panel.y, 3, 3, 4, L"\\ | >|/ |", L" \\   > / ");
    buttonlist[5] = ButtonInitialize(control_panel.x + 9    , control_panel.y, 3, 3, 5, L" _ <o> - ", L"\\_/<X>/-\\");
    buttonlist[6] = ButtonInitialize(79 , 0 , 1, 1, 6, L"X", L"+");
    buttonlist[7] = ButtonInitialize(toolpad.x + 1 , toolpad.y , 3, 3, 7, L" T L8№\\_/", L".(.ГT`L8№");
    buttonlist[8] = ButtonInitialize(78 , 0 , 1, 1, 8, L"O", L"o");
    buttonlist[9] = ButtonInitialize(77 , 0 , 1, 1, 9, L"-", L"_");
    
    buttonlist[10] = ButtonInitialize(rules_editor.x + 14, rules_editor.y + 2, 2, 1, 10, L"/\\", L"№L");
    buttonlist[11] = ButtonInitialize(rules_editor.x + 14, rules_editor.y + 5, 2, 1, 11, L"/\\", L"№L");
    buttonlist[12] = ButtonInitialize(rules_editor.x + 14, rules_editor.y + 8, 2, 1, 12, L"/\\", L"№L");
    buttonlist[13] = ButtonInitialize(rules_editor.x + 14, rules_editor.y + 4, 2, 1, 13, L"\\/", L"`r");
    buttonlist[14] = ButtonInitialize(rules_editor.x + 14, rules_editor.y + 7, 2, 1, 14, L"\\/", L"`r");
    buttonlist[15] = ButtonInitialize(rules_editor.x + 14, rules_editor.y + 10, 2, 1, 15, L"\\/", L"`r");
    
    buttonlist[16] = ButtonInitialize(rules_editor.x + 3, rules_editor.y + 2, 2, 1, 16, L"/\\", L"№L");
    buttonlist[17] = ButtonInitialize(rules_editor.x + 3, rules_editor.y + 4, 2, 1, 17, L"\\/", L"`Г");
    buttonlist[18] = ButtonInitialize(rules_editor.x + 7, rules_editor.y + 2, 2, 1, 18, L"/\\", L"№L");
    buttonlist[19] = ButtonInitialize(rules_editor.x + 7, rules_editor.y + 4, 2, 1, 19, L"\\/", L"`Г");
    buttonlist[20] = ButtonInitialize(rules_editor.x + 9, rules_editor.y + 5, 2, 1, 20, L"/\\", L"№L");
    buttonlist[21] = ButtonInitialize(rules_editor.x + 9, rules_editor.y + 7, 2, 1, 21, L"\\/", L"`Г");
    buttonlist[22] = ButtonInitialize(rules_editor.x + 7, rules_editor.y + 8, 2, 1, 22, L"/\\", L"№L");
    buttonlist[23] = ButtonInitialize(rules_editor.x + 7, rules_editor.y + 10, 2, 1, 23, L"\\/", L"`Г");
    buttonlist[24] = ButtonInitialize(rules_editor.x + 3, rules_editor.y + 8, 2, 1, 24, L"/\\", L"№L");
    buttonlist[25] = ButtonInitialize(rules_editor.x + 3, rules_editor.y + 10, 2, 1, 25, L"\\/", L"`Г");
    buttonlist[26] = ButtonInitialize(rules_editor.x + 1, rules_editor.y + 5, 2, 1, 26, L"/\\", L"№L");
    buttonlist[27] = ButtonInitialize(rules_editor.x + 1, rules_editor.y + 7, 2, 1, 27, L"\\/", L"`Г");
    
    buttonlist[28] = ButtonInitialize(rules_editor.x + 5, rules_editor.y + 5, 2, 1, 28, L"/\\", L"№L");
    buttonlist[29] = ButtonInitialize(rules_editor.x + 5, rules_editor.y + 7, 2, 1, 29, L"\\/", L"`Г");
    
    buttonlist[30] = ButtonInitialize(rules_editor.x + 11, rules_editor.y + 8, 1, 1, 30, L"+", L"*");
    buttonlist[31] = ButtonInitialize(rules_editor.x + 11, rules_editor.y + 10, 1, 1, 31, L"-", L".");
    
    buttonlist[32] = ButtonInitialize(control_panel.x + 13 , control_panel.y, 3, 3, 32, L"Г ` > L №", L"Г ` ‖ L №");
    
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
    FontNumberDraw(f, toolpad.x + 1, toolpad.y, 1, 1, cursor.lm, important_color, 0, 1);
    FontNumberDraw(f, toolpad.x + 3, toolpad.y, 1, 1, cursor.rm, important_color, 0, 1);
}

void ButtonListCheck()
{
    Button_t* b;
    int x, y;
    MouseToGrid(&x, &y);
    if(inpst.mouse.pressed && b_panning == 0 && b_drawing == 0 && b_select_list == 0)
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
            
            b->act_b = 1;
            b->text = 2;
            break;
        case 1:
        
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
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->flag, -1, +1, 1);
            b->act_b = 1;
            b->text = 2;
            break;
        case 11:
            rules_editor.mat_to = 
            cycle(rules_editor.mat_to, 0, mat_amount - 1, +1);
            rules_editor.cond_num = 0;
            b->act_b = 1;
            b->text = 2;
            break;
        case 12:
            rules_editor.cond_num =
            cycle(rules_editor.cond_num, 0, RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->num - 1, +1);
            b->act_b = 1;
            b->text = 2;
            break;
        case 13:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->flag = 
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->flag, -1, +1, -1);
            b->act_b = 1;
            b->text = 2;
            break;
        case 14:
            rules_editor.mat_to = 
            cycle(rules_editor.mat_to, 0, mat_amount - 1, -1);
            rules_editor.cond_num = 0;
            b->act_b = 1;
            b->text = 2;
            break;
        case 15:
            rules_editor.cond_num =
            cycle(rules_editor.cond_num, 0, RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->num - 1, -1);
            b->act_b = 1;
            b->text = 2;
            break;
            
        case 16:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[0] = 
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[0], min_neigh, max_neigh, 1);
            
            b->act_b = 1;
            b->text = 2;
            break;
        case 17:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[0] = 
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[0], min_neigh, max_neigh, -1);
            b->act_b = 1;
            b->text = 2;
            break;
            
        case 18:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[1] = 
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[1], min_neigh, max_neigh, 1);
            b->act_b = 1;
            b->text = 2;
            break;
        case 19:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[1] = 
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[1], min_neigh, max_neigh, -1);
            b->act_b = 1;
            b->text = 2;
            break;
            
        case 20:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[2] = 
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[2], min_neigh, max_neigh, 1);
            b->act_b = 1;
            b->text = 2;
            break;
        case 21:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[2] = 
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[2], min_neigh, max_neigh, -1);
            b->act_b = 1;
            b->text = 2;
            break;
        
        case 22:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[3] = 
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[3], min_neigh, max_neigh, 1);
            b->act_b = 1;
            b->text = 2;
            break;
        case 23:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[3] = 
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[3], min_neigh, max_neigh, -1);
            b->act_b = 1;
            b->text = 2;
            break;
        
        case 24:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[4] = 
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[4], min_neigh, max_neigh, 1);
            b->act_b = 1;
            b->text = 2;
            break;
        case 25:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[4] = 
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[4], min_neigh, max_neigh, -1);
            b->act_b = 1;
            b->text = 2;
            break;
            
        case 26:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[5] = 
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[5], min_neigh, max_neigh, 1);
            b->act_b = 1;
            b->text = 2;
            break;
        case 27:
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[5] = 
            cycle(RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[5], min_neigh, max_neigh, -1);
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
        case 32:
            minimap_speed = 1 - minimap_speed;
            b->act_b = 1;
            b->text = 3 - b->text;
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
        case 32:
            break;
        default:
            b->text = 1;
            break;
        }
    ButtonText(b, b->text);
}

void DisplayPanning(Display_t* d)
{
    int wx, wy, pwx, pwy;
    int x, y, dz, dn, gridx, gridy;
    MouseToGrid(&gridx, &gridy);
    int b_inrec = isinrec(d->grid_x, d->grid_y, d->grid_w, d->grid_h, gridx, gridy);
    
    if(b_inrec && inpst.mouse.down 
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
         && (b_panning || b_inrec))
    {
        printf("\n scroll %i", inpst.mouse.scroll);
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
}

void ScreenInput(Kvad_t* ptr, Display_t* d)
{
    int hexx, hexy, gridx, gridy;
    MouseToGrid(&gridx, &gridy);
    int b_inrec = isinrec(hex_screen.x, hex_screen.y, hex_screen.w, hex_screen.h, gridx, gridy);
    
    if(b_inrec && inpst.mouse.down && b_grab == 0)
        b_drawing = 1;

    if(inpst.mouse.pressed && b_inrec && b_drawing)
    {
        MouseToHex(d, &hexx, &hexy);

        if(inpst.mouse.lmc)
            KvadSetBlob(ptr, hexx, hexy, cursor.lm, cursor.lrad);
        if(inpst.mouse.rmc)
        {
            KvadSetBlob(ptr, hexx, hexy, cursor.rm, cursor.rrad);
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
    sliderlist[0] = SliderInitialize(1, 12, 17, 3, 0, 1, c0, 0);
    sliderlist[1] = SliderInitialize(control_panel.x, control_panel.y + 4, 17, 3, 0, 20, c1, 1);
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
    FontStringDraw(f, s->x + s->c - 1, s->y, 3, 2, L" V [=]", interact_color);
    int value = s->a + s->d * (s->c - 1);
    FontNumberDraw(f, s->x + s->c - 1, s->y + 2, 3, 1, value, important_color, 0, 1);
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
    if(inpst.mouse.pressed && b_panning == 0 && b_drawing == 0 && b_select_list == 0)
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
    fl1 = fopen("../media/ui.txt", "r");
    for(int i = 0; i < 45; i++)
    {
        fgetws(reading, 82, fl1);
        FontStringDraw(f, 0, i, 80, 1, reading, frame_color); 
    }
    fclose(fl1);

    int x, y;
    MouseToGrid(&x, &y);
    FontNumberDraw(f, 1, 0, 2, 1, x, frame_color, 0, 1);
    FontNumberDraw(f, 4, 0, 2, 1, y, frame_color, 0, 1);
    // wchar_t str[240];
    // swprintf(str, 240, L"угол: %i\nсдвиг по х: %i\nсдвиг по y: %i\nмышь х: %i\nмышь y: %i\n", 
    // d->angle, d->hshift.x, d->hshift.y, x, y);
    
    // FontStringDraw(f, 2, 18, 15, 16,
    // str, inform_color);
    
    if(b_ui)
    {
        int r = 16;
        int ang = d->angle;

        int zang, nang, centz, centn;
        centz = hex_screen.x + hex_screen.w / 2 - 1;
        centn = hex_screen.y + hex_screen.h / 2;

        zang = r, nang = 0; 

        HexToGrid(d, &zang, &nang, centz, centn);
        FontNumberDraw(f, zang, nang, 3, 1, 0, inform_color, 0, 1);

        zang = r, nang = -r;
        HexToGrid(d, &zang, &nang, centz, centn);
        FontNumberDraw(f, zang, nang, 3, 1, 90, inform_color, 0, 1);

        zang = 0, nang = -r;
        HexToGrid(d, &zang, &nang, centz, centn);
        FontNumberDraw(f, zang, nang, 3, 1, 120, inform_color, 0, 1);

        zang = -r, nang = 0;
        HexToGrid(d, &zang, &nang, centz, centn);
        FontNumberDraw(f, zang, nang, 3, 1, 180, inform_color, 0, 1);

        zang = -r, nang = r;
        HexToGrid(d, &zang, &nang, centz, centn);
        FontNumberDraw(f, zang, nang, 3, 1, 240, inform_color, 0, 1);

        zang = 0, nang = r;
        HexToGrid(d, &zang, &nang, centz, centn);
        FontNumberDraw(f, zang, nang, 3, 1, 300, inform_color, 0, 1);
    }
    
    if(buttonlist[9]->act_b == 1)
    {
        FontStringDraw(f, 32, 22, 17, 1, L"ОКНО НЕ В ФОКУСЕ!", inform_color);
    }
    
    FontRulesEditorDraw(f);
    
    // FontStringDraw(f, 64, 28, 15, 15, 
    // L"0 - воздух\n1 - верёвка\n2 - огонь\n3 - вода\n4 - песок\n5 - земля\n6 - пар\n7 - лёд\n8 - камень", 
    // inform_color);
    
    // int mat_list_x = 75, mat_list_y = 28;
    
    // for(int i = 0; i < mat_amount; i++)
    // {
    //     HexelDrawOnUI(mat_list_x + 2 * mod(i, 2), mat_list_y + i, i, d->angle, b_ui);
    // }
}

void FontRulesEditorDraw(Font_t* f)
{
    int rules_x = rules_editor.x, rules_y = rules_editor.y;
    int hex_x = 1, hex_y = 2;
    
    FontNumberDraw(f, rules_x + hex_x + 4, rules_y + hex_y + 4,     2, 1, rules_editor.mat_from, important_color, 0, 1);
    
    FontRulesEditorNeighborsDraw(f, rules_x + hex_x + 13  , rules_y + hex_y + 1   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->flag);
    FontNumberDraw(f, rules_x + hex_x + 13  , rules_y + hex_y + 4   , 2, 1, rules_editor.mat_to, important_color, 0, 1);
    FontNumberDraw(f, rules_x + hex_x + 13  , rules_y + hex_y + 7   , 2, 1, rules_editor.cond_num, inform_color, 0, 1);
          
    FontRulesEditorNeighborsDraw(f, rules_x + hex_x + 2   , rules_y + hex_y + 1   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[0]);
    FontRulesEditorNeighborsDraw(f, rules_x + hex_x + 6   , rules_y + hex_y + 1   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[1]);
    FontRulesEditorNeighborsDraw(f, rules_x + hex_x + 8   , rules_y + hex_y + 4   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[2]);
    FontRulesEditorNeighborsDraw(f, rules_x + hex_x + 6   , rules_y + hex_y + 7   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[3]);
    FontRulesEditorNeighborsDraw(f, rules_x + hex_x + 2   , rules_y + hex_y + 7   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[4]);
    FontRulesEditorNeighborsDraw(f, rules_x + hex_x + 0   , rules_y + hex_y + 4   , 2, 1, 
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->req[rules_editor.cond_num]->neighbors[5]);
    
    FontStringDraw(f, rules_x + hex_x + 9, rules_y + hex_y + 1, 3, 1, L"ФЛГ", inform_color);
    FontStringDraw(f, rules_x + hex_x + 11, rules_y + hex_y + 4, 1, 1, L">", inform_color);
    FontStringDraw(f, rules_x + hex_x + 9, rules_y + hex_y + 7, 3, 1, L"УСЛ", inform_color);
    
    int tree_x = 1, tree_y = 12, list_y = 0, list_h = hmax(rules_editor.h - tree_y - hex_x - 1, 1);
    
    list_y = hmax(rules_editor.cond_num - list_h, 0);
    
    
    FontStringDraw(f, rules_x + hex_x + tree_x + 2 , rules_y, 7, 1, L"ИЗ    В", inform_color);
    
    FontNumberDraw(f, rules_x + hex_x + tree_x + 5, rules_y, 2, 1, 
            rules_editor.mat_from, important_color, 0, 1);
    
    FontNumberDraw(f, rules_x + hex_x + tree_x + 10 , rules_y, 2, 1, 
            rules_editor.mat_to, important_color, 0, 1);
            
            
            
    if(rules_editor.cond_num >= rules_editor.list_begin + list_h) rules_editor.list_begin++;
    if(rules_editor.cond_num < rules_editor.list_begin) rules_editor.list_begin--;
    
    rules_editor.list_end = hmin(rules_editor.list_begin + list_h,
            RULES->frommat[rules_editor.mat_from]->tomat[rules_editor.mat_to]->num);
            
    int arrow_y = 
        hmax(
            hmin( rules_editor.y + rules_editor.h - 1, rules_y + hex_x + tree_y + 
                rules_editor.cond_num + 1 - rules_editor.list_begin),
            rules_y + hex_y + tree_y
        );
    FontCharDraw(f, L'>', 
            hmin( rules_editor.x + rules_editor.w, rules_x + hex_x + tree_x - 1 ), 
            arrow_y, inform_color);
    
    for(int number = rules_editor.list_begin; number < rules_editor.list_end; number++)
    {
        FontRulesEditorNeighborsDraw(f, 
            rules_x + hex_x + tree_x, 
            rules_y + hex_y + tree_y + number  - rules_editor.list_begin, 2, 1, 
            RULES->frommat[rules_editor.mat_from]->
            tomat[rules_editor.mat_to]->
            req[number]->flag);
        for(int k = 0; k < 6; k++)
        {
            FontRulesEditorNeighborsDraw(f, 
                rules_x + hex_x + tree_x + 2 + k * 2 , 
                rules_y + hex_y + tree_y + number - rules_editor.list_begin, 2, 1, 
                RULES->frommat[rules_editor.mat_from]->
                tomat[rules_editor.mat_to]->
                req[number]->neighbors[k]);
        }
        
    }
}

void FontRulesEditorNeighborsDraw(Font_t* f, int x, int y, int w, int h, int n)
{
    int other_neigh_color = inform_color;
    if(RULES->frommat[rules_editor.mat_from]->
            tomat[rules_editor.mat_to]->
            req[rules_editor.cond_num]->flag == 0) other_neigh_color = maybe_color;
    if(n >= 0)  FontNumberDraw(f, x, y, w, h, n, yes_color, 1, 1);
    else if(n == -1) FontStringDraw(f, x, y, w, h, L"--", other_neigh_color);
    else if(n == -2) FontStringDraw(f, x, y, w, h, L"-0", no_color);
    else FontNumberDraw(f, x, y, w, h, n + 2, no_color, 1, 1);
    
}

void FontPrintToInfoBox(Font_t* f)
{
    int lines = 0;
    int x, y;
    MouseToGrid(&x, &y);
    
    if((inpst.mouse.scroll) &&
        isinrec(info_box.x, info_box.y, info_box.w, info_box.h, x, y ))
    {
        info_box.shift -= inpst.mouse.scroll;
    }
    lines = StringLinesCount(info_box.w, info_box.s);
    info_box.shift = hmin
    (
        hmax
        (
            info_box.shift,
            0
        ), lines - info_box.h
    );
    
    FontStringSHiftDraw(f, info_box.x, info_box.y, 
        info_box.w, info_box.h, info_box.s, inform_color, info_box.shift);
    
}

void InfoBoxUpdate()
{
    int x, y;
    MouseToGrid(&x, &y);
    
    if(isinrec(rules_editor.x, rules_editor.y, rules_editor.w, rules_editor.h, x, y ))
    {
        info_box.s = s_rules_editor;
    }
    else if(isinrec(toolpad.x, toolpad.y, toolpad.w, toolpad.h, x, y ) ||
            isinrec(select_list.rectangle.x, select_list.rectangle.y, 
                select_list.rectangle.w, select_list.rectangle.h, x, y ))
    {
        info_box.s = s_toolpad;
    }
    else if(isinrec(control_panel.x, control_panel.y, control_panel.w, control_panel.h, x, y ))
    {
        info_box.s = s_control_panel;
    }
    else if(!isinrec(info_box.x, info_box.y, info_box.w, info_box.h, x, y ))
    {
        info_box.s = L"";
    }
}

void DisplayListUpdate()
{
    DisplayPanning(displaylist[0]);
    
    // displaylist[1]->angle = displaylist[0]->angle;
    
    // displaylist[1]->hshift.x = displaylist[0]->hshift.x
    //  * displaylist[0]->scale / displaylist[1]->scale;
    // displaylist[1]->screen_shift.x = displaylist[0]->screen_shift.x
    //  * displaylist[0]->scale / displaylist[1]->scale;
    
    // displaylist[1]->hshift.y = displaylist[0]->hshift.y
    //  * displaylist[0]->scale / displaylist[1]->scale;
    // displaylist[1]->screen_shift.y = displaylist[0]->screen_shift.y
    //  * displaylist[0]->scale / displaylist[1]->scale;
}

void SelectListDraw(Font_t* f, Display_t* d, Select_List_t *p_sl)
{
    int lines;
    int x, y;
    MouseToGrid(&x, &y);
    
    if(isinrec(p_sl->rectangle.x, p_sl->rectangle.y, p_sl->rectangle.w, p_sl->rectangle.h, x, y )
         && (inpst.mouse.scroll))
    {
        p_sl->shift -= inpst.mouse.scroll;
        inpst.mouse.scroll = 0;
    }
    lines = p_sl->max + 1 - p_sl->min;
    p_sl->shift = hmin
    (
        hmax
        (
            p_sl->shift,
            0
        ), lines - p_sl->rectangle.h
    );
    FontStringSHiftDraw(f, p_sl->rectangle.x + 5, p_sl->rectangle.y, p_sl->rectangle.w - 5, p_sl->rectangle.h, p_sl->s, inform_color, p_sl->shift); 
    
    for(int i = p_sl->shift; i < p_sl->rectangle.h + p_sl->shift; i++)
    {
        HexelDrawOnUI(p_sl->rectangle.x + 2 * mod(i, 2), 
            p_sl->rectangle.y + i - p_sl->shift, i, d->angle, b_ui);
    }
    
    int lm_arrow, rm_arrow;
    lm_arrow = cursor.lm - p_sl->shift;
    rm_arrow = cursor.rm - p_sl->shift;
    
    if(lm_arrow == rm_arrow)
        FontStringDraw(f, p_sl->rectangle.x + 4, p_sl->rectangle.y + lm_arrow, 
            1, 1, L"Д", inform_color);
    else
    {
        if(lm_arrow >= 0 && lm_arrow < p_sl->rectangle.h)
            FontStringDraw(f, p_sl->rectangle.x + 4, p_sl->rectangle.y + lm_arrow, 
                1, 1, L"Л", inform_color);
        if(rm_arrow >= 0 && rm_arrow < p_sl->rectangle.h)
            FontStringDraw(f, p_sl->rectangle.x + 4, p_sl->rectangle.y + rm_arrow, 
                1, 1, L"П", inform_color);
    }
}

void SelectListUpdate()
{
    Select_List_t *s = &select_list;
    int x, y, c;
    MouseToGrid(&x, &y);
    if(inpst.mouse.pressed && b_panning == 0 && b_drawing == 0)
    {
        if( (isinrec(s->rectangle.x, s->rectangle.y, s->rectangle.w, s->rectangle.h, x, y) == 1
                && b_button == -1 && b_slider == -1) )
        {
            c = hmax(y - s->rectangle.y + s->shift, 0);
            if(inpst.mouse.lmc)
                cursor.lm = c;
            if(inpst.mouse.rmc)
                cursor.rm = c;
            b_select_list = 1;
        }
    }
    else
    {
        b_select_list = 0;
    }
}