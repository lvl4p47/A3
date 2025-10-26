#include "interface.h"

Kvad_t* k1;
Display_t* d1;
Font_t* f1;
Button_t* b1;
Node_t* n1;

int64_t delta_rend(uint64_t ft, uint64_t t_f)
{
	return t_f - ft;
}

int64_t delta_step(uint64_t st, uint64_t t_s)
{
	return t_s - st;
}

void update_frame(uint64_t* ft, int64_t* dr)
{
	InputRegister();
    InputUpdate();
    
    //AudioUpdate(k1, b_pause);

    DisplayListUpdate();
    ScreenInput(k1, displaylist[0]);

    ScreenClear();

    DisplayListDraw(k1, b_ui);
    
    InterfaceUpdate(k1);
    InterfaceDraw(f1, displaylist[0]);

    ScreenRefresh();
	*ft = (int64_t)SDL_GetPerformanceCounter();
	*dr = 0;
}

void update_step(uint64_t* st, int64_t* ds)
{
    if(b_pause == 0 || b_step == 1)
    {
        KvadUpdate(k1);
        
        
        b_step = 0;
    }
	*st = (int64_t)SDL_GetPerformanceCounter();
	*ds = 0;
}

void calculate_delta(int64_t* dr, int64_t* ds, uint64_t ct, uint64_t ft,
                    uint64_t st, uint64_t t_f, uint64_t t_s)
{
    *dr = delta_rend(ct - ft, t_f);
	*ds = delta_step(ct - st, t_s);
}

void delay(int64_t dr, int64_t ds)
{
	int64_t dt = hmax(0, hmin(dr, ds)) * 1000 / SDL_GetPerformanceFrequency();
	SDL_Delay(dt + 1);

}

int main( int argc, char * argv[] )
{
    int64_t dr, ds;
	uint64_t frame_time, step_time, cur_time;

	frame_time = SDL_GetPerformanceCounter();
	step_time = SDL_GetPerformanceCounter();

    AudioInitialize();
    VideoInitialize();
    InputInitialize();
    HexagonsInitialize();
    EntitySystemInitialize();
    
    k1 = KvadInitialize(side, side);
    DisplayListInitialize();
    f1 = FontInitialise();
    InterfaceInitialize();
    
    MeatListInitialize(k1);
    
    // KvadSetBlob(k1, 10, 10, 10, 3);

    while(!b_quit)
    {
        cur_time = SDL_GetPerformanceCounter();
		calculate_delta(&dr, &ds, cur_time, frame_time, step_time, t_f, t_s);

		if (ds <= 0) {
			update_step(&step_time, &ds);
		}

		if (dr <= 0) {
			update_frame(&frame_time, &dr);
		}

		calculate_delta(&dr, &ds, cur_time, frame_time, step_time, t_f, t_s);
		delay(dr, ds);
    }
    MeatListTerminate(k1);
    EntitySystemTerminate();
    KvadTerminate(k1);
    DisplayListTerminate();
    FontTerminate(f1);
    InterfaceTerminate();
    HexagonsTerminate();
    InputTerminate();
    VideoTerminate();
    AudioTerminate();

    return 1;
}