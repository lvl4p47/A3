
#include "func.h"

//FUNC

//INPUT


//HEXAGONS
int side = 96;

//VIDEO

int trig[48] =
{
        0, 1, 2, 3, 4, 5, 6, 6, 7, 7, 8, 8,
        8, 8, 8, 7, 7, 6, 6, 5, 4, 3, 2, 1,
        0,-1,-2,-3,-4,-5,-6,-6,-7,-7,-8,-8,
       -8,-8,-8,-7,-7,-6,-6,-5,-4,-3,-2,-1
};

int hsin(int a)
{
    return trig[a % 48];
}

int hcos(int a)
{
    return trig[(a + 12) % 48];
}

int64_t hmax(int64_t a, int64_t b) {
	if (a > b) return a;
	else return b;
}

int64_t hmin(int64_t a, int64_t b) {
	if (a < b) return a;
	else return b;
}

int mod(int a, int t)
{
    int ans;
    ans = a % t;
	ans += ans < 0 ? t : 0;
    return ans;
}

int cycle(int a, int min, int max, int d)
{
    return mod(a - min + d, max - min + 1) + min;
}

int hdiv(int a, int t)
{
    int ans;
    ans = (a - mod(a, t)) / t;
    return ans;
}

void swap(int* a, int* b)
{
    int temp = *b;
    *b = *a;
    *a = temp;
}

int isinrec(int x, int y, int w, int h, int cx, int cy)
{
    int ret = 0;
    
    if(cx >= x && cy >= y && cx < x + w && cy < y + h)
        ret = 1;

    return ret;
}

int hlog(int a, double p)
{
    if(p <= 0) return -1;
    if(p == 1) return -1;
    double a_d = a;
    int log = 0;
    while(a_d > p)
    {
        a_d /= p;
        log++;
    }
    return log;
}

void PixelToHex(Display_t* d, int *z, int *n)
{
    int a = d->angle;
    int det = hcos(a) * hsin(a + 8) - hsin(a) * hcos(a + 8);

    int x, y;
    x = *z, y = *n;

    x += -4 -d->screen.x - d->screen.w / 2 - d->screen_shift.x;
    y += -4 -d->screen.y - d->screen.h / 2 - d->screen_shift.y;

    *z = hdiv( x * hsin(a + 8) - y * hcos(a + 8), det );
    *n = hdiv(-x * hsin(  a  ) + y * hcos(  a  ), det );

    x -= *z * hcos(a) + *n * hcos(a + 8);
    y -= *z * hsin(a) + *n * hsin(a + 8);
    
    int c0 = hcos(a);
    int c8 = hcos(a + 8);
    int s0 = hsin(a);
    int s8 = hsin(a + 8);

    int mz = 0;
    int mn = 0;
    int kz = 0;
    int kn = 0;

    int pnts[5][4] =
    {
        {1, 0,-1, 1},
        {1,-1, 0, 1},
        {0, 0, 1, 1},
        {2, 0, 0, 1},
        {1, 0, 0, 2}
    };

    int divisor;

    int A[5], B[5];
    for(int i = 0; i < 5; i++)
    {
        mz = pnts[i][0];
        mn = pnts[i][1];
        kz = pnts[i][2];
        kn = pnts[i][3];
        if(i == 2)
        {
            divisor = 1;
        }
        else
        {
            divisor = mn * kz - mz * kn;
        }
        A[i] = (s0 * pnts[i][0] + s8 * pnts[i][1] - 
                s0 * pnts[i][2] - s8 * pnts[i][3]) / divisor;
        B[i] = (c0 * pnts[i][2] + c8 * pnts[i][3] - 
                c0 * pnts[i][0] - c8 * pnts[i][1]) / divisor;
    }

    
    int line1 = det, line2 = det, line3 = 0, line4 = det, line5 = det;
    int ang = a % 24;
    if(ang == 0) // v
    {
        line1 += 4;
        line2 += 4;
        line3 -= 4;
        line4 -= 4;
        line5 += 4;
    }
    if(ang == 1) // v
    {
        line2 += 4;
        line5 -= 2;
        line4 -= 6;
    }
    if(ang == 2) // v
    {
        line1 += 3;
        line4 -= 1;
    }
    if(ang == 3) // v
    {
        line1 += 4;
        line5 -= 5;
    }
    if(ang == 4) // v
    {
        line1 += 4;
        line2 += 4;
        line3 += 4;
        line4 += 4;
        line5 -= 4;
    }
    if(ang == 5) // v
    {

    }
    if(ang == 6) // v
    {
        line1 += 1;
        line3 -= 1;
    }
    if(ang == 7) // v
    {
        line3 -= 4;
        line4 -= 4;
        line5 -= 4;
    }
    if(ang == 8) // v
    {
        line5 += 4;
        line1 -= 4;
    }
    if(ang == 9) // v
    {
        line3 -= 4;
        line4 -= 4;
        line5 -= 4;
    }
    if(ang == 10) // v
    {
        line2 += 4;
        line3 += 1;
    }
    if(ang == 11) // v
    {
        
    }
    if(ang == 12) // v
    {
        line1 += 4;
        line2 += 4;
        line3 -= 4;
        line4 -= 4;
        line5 += 4;
    }
    if(ang == 13) // v
    {
        line2 += 4;
        line4 -= 6;
    }
    if(ang == 14) // v
    {
        line2 += 4;
        line3 += 4;
    }
    if(ang == 15) // v
    {
        //line1 += 4;
        line4 -= 6;
        line5 -= 6;
    }
    if(ang == 16) // v
    {
        line1 -= 4;
        line2 -= 4;
        line3 -= 4;
        line4 -= 4;
        line5 -= 6;
    }
    if(ang == 17) // v
    {
        line1 -= 4;
        line4 -= 4;
    }
    if(ang == 18) // v
    {
        line1 += 4;
    }
    if(ang == 19) // v
    {
        line3 -= 4;
        line4 -= 4;
        line5 -= 4;
    }
    if(ang == 20) // v
    {
        line1 -= 4;
        line5 += 4;
    }
    if(ang == 21) // v
    {
        line3 -= 4;
        line4 -= 4;
        line5 -= 4;
    }
    if(ang == 22) // v
    {
        line1 += 4;
        line4 += 4;
    }
    if(ang == 23) // v
    {
        line2 -= 4;
        line5 -= 4;
    }
    


    int conds[5] = 
    {
                A[0] * x + B[0] * y < line1,
                A[1] * x + B[1] * y < line2,
                A[2] * x + B[2] * y < line3,
                A[3] * x + B[3] * y < line4,
                A[4] * x + B[4] * y < line5
    };

    int test = 1;

    *z = *z * test + 1, *n = *n * test + 1;
    if(conds[0] == 1 && conds[1] == 1)
    {
        *z = *z - 1, *n = *n - 1;
    }
    if (conds[1] == 0 && conds[2] == 1 && conds[3] == 1)
    {
        *n = *n - 1;
    }
    if (conds[0] == 0 && conds[2] == 0 && conds[4] == 1)
    {
        *z = *z - 1;
    }
    if (conds[3] == 0 && conds[4] == 0)
    {
        
    }
    
}

void HexToGrid(Display_t* d, int *z, int *n, int centz, int centn)
{
    int xnew, ynew;
    xnew = centz +
    *z * hcos(d->angle) / 8 + *n * hcos(d->angle + 8) / 8;
    ynew = centn +
    *z * hsin(d->angle) / 8 + *n * hsin(d->angle + 8) / 8;
    *z = xnew;
    *n = ynew;
}

int hdist(int az, int an, int bz, int bn)
{
    return hmax(abs(az - bz), hmax(abs(an - bn), abs( (az + an) - (bz + bn) )));
}