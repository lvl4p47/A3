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
            ptr->arr[i][j].tmp = 0;
            ptr->arr[i][j].num = 0;
            ptr->arr[i][j].fld = 0;
            ptr->arr[i][j].vel = 0;
            ptr->arr[i][j].st8 = 0;
        }
    }
}

void KvadSetMat(Kvad_t* ptr, int z, int n, int value)
{
    Cell_t* cptr;
    cptr = KvadGetHexel(ptr, z, n);

    cptr->mat = value;
    
    switch (value)
    {
    case 0:
        cptr->st8 = 0;
        cptr->clr = 7;
        break;
    case 1:
        cptr->st8 = 2;
        cptr->clr = 7;
        break;
    case 2:
        cptr->st8 = 2;
        cptr->clr = 1;
        break;
    case 3:
        cptr->st8 = 1;
        cptr->clr = 3;
        break;
    default:
        cptr->st8 = 2;
        cptr->clr = 7;
        break;
    }
    
}

Cell_t* KvadGetHexel(Kvad_t* ptr, int z, int n)
{
    z = z % ptr->width;
	z += z < 0 ? ptr->width : 0;
    n = n % ptr->height;
    n += n < 0 ? ptr->height : 0;

    return &ptr->arr[n][z];
}

void KvadSetBlob(Kvad_t* ptr, int z, int n, int value, int rad)
{
    for(int i = -rad; i <= rad; i++)
    {
        for(int j = -rad; j <= rad; j++)
        {
            if( i + j >= -rad && i + j <= rad)
                KvadSetMat(ptr, z + j, n + i, value);
        }
    }
}

void KvadUpdate(Kvad_t* ptr)
{
    int dz, dn;
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            
            KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
            if(KvadGetHexel(ptr, j, i)->st8 == 1)
            {
                Repulsion(ptr, j, i, 1, &dz, &dn);
                if(KvadGetHexel(ptr, j + dz, i + dn)->fld == 0 && KvadGetHexel(ptr, j + dz, i + dn)->mat == 0)
                {
                    KvadGetHexel(ptr, j + dz, i + dn)->fld = 1;
                }
                else
                {
                    KvadGetHexel(ptr, j + dz, i + dn)->fld = -1;
                }
            }
        }
    }

    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            if(KvadGetHexel(ptr, j, i)->st8 == 1)
            {
                Repulsion(ptr, j, i, 1, &dz, &dn);
                if(KvadGetHexel(ptr, j + dz, i + dn)->fld == 1)
                {
                    swap(&KvadGetHexel(ptr, j     , i     )->tmp,
                        &KvadGetHexel(ptr, j + dz, i + dn)->tmp);
                }
            }
        }
    }

    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            
            KvadGetHexel(ptr, j, i)->fld = 0;
            KvadSetMat(ptr, j, i, KvadGetHexel(ptr, j, i)->tmp);
        }
    }
}

int NeighbourCount(Kvad_t* ptr, int z, int n)
{
    int N = 0;
    int type = KvadGetHexel(ptr, z, n)->mat;
    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {
            if( i - j != 0)
                
                N += (KvadGetHexel(ptr, z + j, n + i)->mat == 1) ? 1 : 0;
        }
    }
    return N;
}

int ReflectionAngle(Kvad_t* ptr, int z, int n, int val, int dirz, int diry)
{
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if( i - j != 0)
            {
                
            }
        }
    }
    return 0; 
}

void Repulsion(Kvad_t* ptr, int z, int n, int val, int* dz, int* dn)
{
    *dz = 0, *dn = 0;
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if( i - j != 0)
            {
                if(KvadGetHexel(ptr, z + j, n + i)->st8 != 0)
                {
                    *dz -= j;
                    *dn -= i;
                }
            }
        }
    }
    /*
    if(*dz - *dn == 0 || 2 * *dz + *dn == 0 || *dz + 2 * *dn == 0)
    {
        *dz = 0;
        *dn = 0;
    }
    else
    {
        if(*dz == 2) *dz = 1;
        if(*dn == 2) *dn = 1;
        if(*dz == -2) *dz = -1;
        if(*dn == -2) *dn = -1;
    }
    */
}