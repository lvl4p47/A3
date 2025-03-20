#include "interface.h"

extern int side;

Kvad_t* k1;
Display_t* d1;
Font_t* f1;
Button_t* b1;


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

    DisplayPanning(k1, d1);
    ScreenInput(k1, d1);

    ScreenClear();

    /*
    int tx, ty;
    int sx = 61 * 8 + 72, tw = 144;
    int sy = 1 * 8 + 72, th = 144;
    
    for(int i = sy - th / 2; i < sy + th / 2; i++)
    {
        for(int j = sx - tw / 2; j < sx + tw / 2; j++)
        {
            tx = j, ty = i;
            PixelToHex(d1, &tx, &ty);
            point(61 * 0 + j, 1 * 0 + i, 85 * mod(tx, 3) * mod(tx, 2), mod(ty,2) * mod(ty, 3) * 85, mod(tx + ty, 2) * mod(tx + ty, 3) * 85);
            //point(61 * 0 + j, 1 * 0 + i, tx * 255, ty * 255, ty * 255 - tx * 255);
            //mod(ty, 3) * 85 * 0, mod(tx - 1, 2) * 255 - mod(ty, 2) * 255
        }
    }
    */

    KvadRender(k1, d1, 1, 1, b_ui);
    
    InterfaceUpdate();

    //
    InterfaceDraw(f1, d1);
    //ButtonListDraw(f1);
    //ButtonDraw(b1, f1);

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
    uint64_t fps = 60;
    uint64_t sps = 60;

    //uint64_t t_f = SDL_GetPerformanceFrequency() / fps;
    //uint64_t t_s = SDL_GetPerformanceFrequency() / sps;

    int64_t dr, ds;
	uint64_t frame_time, step_time, cur_time;

	frame_time = SDL_GetPerformanceCounter();
	step_time = SDL_GetPerformanceCounter();

    VideoInitialize();
    InputInitialize();

    k1 = KvadInitialize(side, side);
    d1 = DisplayInitialize(17, 1, 43, 43);
    f1 = FontInitialise();
    InterfaceInitialize();
    //b1 = ButtonInitialize(1, 1, 3, 3, L"000000000");


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

    KvadTerminate(k1);
    DisplayTerminate(d1);
    FontTerminate(f1);
    InterfaceTerminate();
    VideoTerminate();

    return 1;
}