#include "hexagons.h"

Kvad_t* KvadInitialize(int width, int height)
{
    Kvad_t* ptr = (Kvad_t*)malloc(sizeof(Kvad_t));

    ptr->width = width;
    ptr->height = height;
    ptr->arr = (Cell_t**)malloc(height * sizeof(Cell_t*));

	for (int i = 0; i < height; i++)
	{
		ptr->arr[i] = (Cell_t*)malloc(width * sizeof(Cell_t));
    }

    KvadZero(ptr);

    return ptr;
}

void KvadTerminate(Kvad_t* ptr)
{
    for (int i = 0; i < ptr->height; i++)
    {
		free(ptr->arr[i]);
    }
	free(ptr->arr);
	free(ptr);
}

void KvadZero(Kvad_t* ptr)
{
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            ptr->arr[i][j].mat = 0;
            ptr->arr[i][j].num = 0;
        }
    }
}

void KvadSetHexel(Kvad_t* ptr, int z, int n, int value)
{
    z = z % ptr->width;
	z += z < 0 ? ptr->width : 0;
    n = n % ptr->height;
    n += n < 0 ? ptr->height : 0;

    ptr->arr[n][z].mat = value;
}

void KvadSetBlob(Kvad_t* ptr, int z, int n, int value, int rad)
{
    for(int i = -rad; i <= rad; i++)
    {
        for(int j = -rad; j <= rad; j++)
        {
            if( i + j >= -rad && i + j <= rad)
                KvadSetHexel(ptr, z + j, n + i, value);
        }
    }
}

void KvadUpdate(Kvad_t* ptr)
{
    for(int i = 1; i < ptr->height - 1; i++)
    {
        for(int j = 1; j < ptr->width - 1; j++)
        {
            ptr->arr[i][j].num = CountNeighbour(ptr, j, i);
            //ptr->arr[i][j].num = 1;
        }
    }

    for(int i = 1; i < ptr->height - 1; i++)
    {
        for(int j = 1; j < ptr->width - 1; j++)
        {
            //ptr->arr[i][j].mat = ptr->arr[i][j].num;

            switch(ptr->arr[i][j].num)
            {
            case 0:
                ptr->arr[i][j].mat = 0;
                break;
            case 1:
                ptr->arr[i][j].mat = 0;
                break;
            case 2:
                ptr->arr[i][j].mat = 0;
                break;
            case 3:
                //ptr->arr[i][j].mat = 1;
                break;
            case 4:
                ptr->arr[i][j].mat = 1;
                break;
            case 5:
                ptr->arr[i][j].mat = 1;
                break;
            case 6:
                ptr->arr[i][j].mat = 1;
                break;

            }

        }
    }
}

int CountNeighbour(Kvad_t* ptr, int z, int n)
{
    int N = 0;
    int type = ptr->arr[z][n].mat;
    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {
            if( i - j != 0)
                N += (ptr->arr[n + i][z + j].mat == 1) ? 1 : 0;
        }
    }
    return N;
}
