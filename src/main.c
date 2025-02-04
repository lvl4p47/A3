#include "text.h"

extern int quit;
extern int side;

Kvad_t* k1;
Display_t* d1;
Font_t* f1;
wchar_t reading[82];

int64_t delta_rend(uint64_t ft, uint64_t t_f) {
	return t_f - ft;
}

int64_t delta_step(uint64_t st, uint64_t t_s) {
	return t_s - st;
}

void update_frame(uint64_t* ft, int64_t* dr) {
	//printf("\nframe");

	InputRegister();
    InputUpdate();

    KvadPanning(k1, d1);

    ScreenClear();

    KvadRender(k1, d1, 1, 1);

    FILE* fl1 = NULL;
    fl1 = fopen("./src/ui.txt", "r");
    for(int i = 0; i < 45; i++)
    {
        fgetws(reading, 82, fl1);
        FontStringDraw(f1, 0, i, 80, 1, reading); 
    }
    fclose(fl1);

    int x, y;
    MouseToGrid(&x, &y);
    wchar_t str[240];
    swprintf(str, 240, L"угол: %i\nсдвиг по х: %i\nсдвиг по y: %i\nмышь х: %i\nмышь y: %i\n", 
    d1->angle, d1->hshift.x, d1->hshift.y, x, y);
    
    FontStringDraw(f1, 1, 28, 15, 16,
    str);

    ScreenRefresh();
	*ft = (int64_t)SDL_GetPerformanceCounter();
	*dr = 0;
}

void update_step(uint64_t* st, int64_t* ds) {
	//printf("\t step");

    KvadUpdate(k1);

	*st = (int64_t)SDL_GetPerformanceCounter();
	*ds = 0;
}

void calculate_delta(int64_t* dr, int64_t* ds, uint64_t ct, uint64_t ft,
                    uint64_t st, uint64_t t_f, uint64_t t_s) {
    *dr = delta_rend(ct - ft, t_f);
	*ds = delta_step(ct - st, t_s);

}

void delay(int64_t dr, int64_t ds) {
	int64_t dt = hmax(0, hmin(dr, ds)) * 1000 / SDL_GetPerformanceFrequency();
	SDL_Delay(dt + 1);

}

int main( int argc, char * argv[] )
{
    uint64_t fps = 60;
    uint64_t sps = 6;

    uint64_t t_f = SDL_GetPerformanceFrequency() / fps;
    uint64_t t_s = SDL_GetPerformanceFrequency() / sps;

    int64_t dr, ds;
	uint64_t frame_time, step_time, cur_time;

	frame_time = SDL_GetPerformanceCounter();
	step_time = SDL_GetPerformanceCounter();

    VideoInitialize();
    InputInitialize();

    k1 = KvadInitialize(side, side);
    d1 = DisplayInitialize(17, 1, 43, 43);

    f1 = FontInitialise();


    while(!quit)
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

#if 0
        InputRegister();
        InputUpdate();

        KvadPanning(k1, d1);

        ScreenClear();
        KvadRender(k1, d1, 1, 1);

        KvadUpdate(k1);

        SDL_Delay( 16 );
#endif
    }

    ScreenClear();

    KvadTerminate(k1);
    DisplayTerminate(d1);
    VideoTerminate();

    return 1;
}
