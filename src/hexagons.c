#include "hexagons.h"

int mat_amount = 9;
int*** p_rules;
int* neighbours_required;

int Yrot[6] = {1, 0, -1, -1, 0, 1};

int t;

void RulesInitialize()
{
    p_rules = malloc(7 * mat_amount * mat_amount * sizeof(int**));
    for(int i = 0; i < mat_amount; i++)
    {
        p_rules[i] = malloc(7 * mat_amount * sizeof(int*));
        for(int j = 0; j < mat_amount; j++)
        {
            p_rules[i][j] = malloc(7 * sizeof(int));
            for(int k = 0; k < 7; k++)
            {
                p_rules[i][j][k] = -1;
            }
        }
    }
    neighbours_required = malloc(mat_amount * sizeof(int));    

    p_rules[1][2][0] = 0;
    p_rules[1][2][1] = 2;
    p_rules[1][2][2] = 0;

    p_rules[2][0][0] = 0;
    
    p_rules[3][7][0] = 0;
    p_rules[3][7][1] = 7;
    p_rules[3][7][2] = 7;

    p_rules[3][6][0] = 0;
    p_rules[3][6][1] = 2;
    
    p_rules[4][5][0] = 0;
    p_rules[4][5][1] = 3;

    p_rules[5][4][0] = 0;
    p_rules[5][4][1] = 2;
    
    p_rules[6][3][0] = 0;
    p_rules[6][3][1] = 6;
    p_rules[6][3][2] = 6;
    p_rules[6][3][3] = 6;
    p_rules[6][3][4] = 6;
    p_rules[6][3][5] = 6;
    p_rules[6][3][6] = 6;
    
    p_rules[6][7][0] = 0;
    p_rules[6][7][1] = 7;
    
    p_rules[7][3][0] = 0;
    p_rules[7][3][1] = 2;
    
}

void RulesTerminate()
{
    for(int i = 0; i < mat_amount; i++)
    {
        for(int j = 0; j < mat_amount; j++)
        {
            free(p_rules[i][j]);
        }
        free(p_rules[i]);
    }
    free(p_rules);
    free(neighbours_required);
}

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
    t = 0;

    RulesInitialize();

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

    RulesTerminate();
}

void KvadZero(Kvad_t* ptr)
{
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            ptr->arr[i][j].mat = 0;
            ptr->arr[i][j].tmp = 0;
            ptr->arr[i][j].prs_grav = 0;
            ptr->arr[i][j].prs_dist = 0;
            ptr->arr[i][j].fld = 0;
            ptr->arr[i][j].fld2 = 0;
            ptr->arr[i][j].dx = 0;
            ptr->arr[i][j].dy = 0;
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
        cptr->st8 = 5;
        cptr->clr = 2;
        break;
    case 2:
        cptr->st8 = 2;
        cptr->clr = 1;
        break;
    case 3:
        cptr->st8 = 6;
        cptr->clr = 3;
        break;
    case 4:
        cptr->st8 = 3;
        cptr->clr = 6;
        break;
    case 5:
        cptr->st8 = 4;
        cptr->clr = 1;
        break;
    case 6:
        cptr->st8 = 1;
        cptr->clr = 7;
        break;
    case 7:
        cptr->st8 = 4;
        cptr->clr = 4;
        break;
    default:
        cptr->st8 = 2;
        cptr->clr = 7;
        break;
    }
}

Cell_t* KvadGetHexel(Kvad_t* ptr, int z, int n)
{
    z = mod(z, ptr->width);
    n = mod(n, ptr->height);

    return &ptr->arr[n][z];
}

void KvadSetBlob(Kvad_t* ptr, int z, int n, int value, int rad)
{
    for(int i = -rad; i <= rad; i++)
    {
        for(int j = -rad; j <= rad; j++)
        {
            if( i + j >= -rad && i + j <= rad)
            {
                if(KvadGetHexel(ptr, z + j, n + i)->mat == 0 || value == 0)
                {
                    
                    KvadSetMat(ptr, z + j, n + i, value);
                    KvadGetHexel(ptr, z + j, n + i)->prs_grav = 0;
                    KvadGetHexel(ptr, z + j, n + i)->prs_dist = 0;
                    
                    //KvadGetHexel(ptr, z + j, n + i)->prs_dist = value;
                }
            }
        }
    }
}

void WaveUpdate(Kvad_t* ptr)
{

    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->prs_dist;
        }
    }
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            if(KvadGetHexel(ptr, j, i)->prs_dist >= 6)
            {
                for(int di = -1; di < 2; di++)
                {
                    for(int dj = -1; dj < 2; dj++)
                    {
                        if( di - dj != 0)
                        {
                            if( KvadGetHexel(ptr, j + dj, i + di)->prs_dist == 0 || 1)
                            {
                            
                                KvadGetHexel(ptr, j + dj, i + di)->tmp += 1;//KvadGetHexel(ptr, j, i)->prs_dist - 1;
                            }
                        }
                    }
                }
                KvadGetHexel(ptr, j, i)->tmp -= 6;
                /*
                for(int di = -1; di < 2; di++)
                {
                    for(int dj = -1; dj < 2; dj++)
                    {
                        if( di - dj != 0)
                        {
                            KvadGetHexel(ptr, j + dj, i + di)->tmp += 1;
                            KvadGetHexel(ptr, j, i)->tmp -= 1;
                        }
                    }
                }
                */
                
            }
        }
    }
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            KvadGetHexel(ptr, j, i)->prs_dist = KvadGetHexel(ptr, j, i)->tmp;
            /*
            if(KvadGetHexel(ptr, j, i)->prs_nongrav != 0)
                KvadSetMat(ptr, j, i, 2);
            else
                KvadSetMat(ptr, j, i, 0);
            KvadGetHexel(ptr, j, i)->clr = 1 + mod(KvadGetHexel(ptr, j, i)->tmp, 7);
            */
        }
    }
}

void SolidUpdate(Kvad_t* ptr)
{
    int b_change = 0;
    
    for(int i = 0; i < mat_amount; i++)
    {
        neighbours_required[i] = 0;
    }
    
    int cen;
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
            cen = KvadGetHexel(ptr, j, i)->mat;
            for(int new = 0; new < mat_amount; new++)
            {
                if(p_rules[cen][new][0] != -1)
                {
                    for(int i = 0; i < mat_amount; i++)
                    {
                        neighbours_required[i] = 0;
                    }
                    int cur_neighbour_mat;
                    for(int n = 1; n < 7; n++)
                    {
                        if(p_rules[cen][new][n] != -1)
                        {
                            cur_neighbour_mat = p_rules[cen][new][n];
                            neighbours_required[cur_neighbour_mat]++; 
                        }
                    }
                    int neighbour_amount;
                    for(int neighbour_mat = 0; neighbour_mat < mat_amount; neighbour_mat++)
                    {
                        neighbour_amount = NeighbourCount(ptr, j, i, neighbour_mat);
                        if(neighbour_amount >= neighbours_required[neighbour_mat])
                        {
                            b_change = 1;
                        }
                        else
                        {
                            b_change = 0;
                            neighbour_mat = mat_amount;
                        }
                    }
                    if(b_change)
                    {
                        KvadGetHexel(ptr, j, i)->tmp = new;
                        b_change = 0;
                    }
                }
            }
        }
    }
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            KvadSetMat(ptr, j, i, KvadGetHexel(ptr, j, i)->tmp);
        }
    }
}

void GasUpdate(Kvad_t* ptr)
{
    int dz, dn;
    int gx = 0, gy = 1;
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            
            KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
            if(KvadGetHexel(ptr, j, i)->st8 == 1)
            {
                Repulsion(ptr, j, i, gx, gy, &dz, &dn, 0);
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
                Repulsion(ptr, j, i, gx, gy, &dz, &dn, 0);
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

void SandUpdate(Kvad_t* ptr)
{
    int gx = 0, gy = 1;
    int dz = 0, dn = 0;
    int f_direct = 0;
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            
            KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
            if(KvadGetHexel(ptr, j, i)->st8 == 3)
            {
                Force(ptr, j, i, gx, gy, &dz, &dn);
                f_direct = 0;
                if(dz == gx && dn == gy)
                {
                    f_direct = 1;
                    KvadGetHexel(ptr, j + dz, i + dn)->fld = 2;
                }
                else 
                {
                    if( KvadGetHexel(ptr, j + dz, i + dn)->fld == 0
                      && KvadGetHexel(ptr, j + dz, i + dn)->mat == 0)
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = 1;
                    }
                    else if( KvadGetHexel(ptr, j + dz, i + dn)->fld == 1)
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = -1;
                    }
                }
            }
        }
    }

    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            if(KvadGetHexel(ptr, j, i)->st8 == 3)
            {
                Force(ptr, j, i, gx, gy, &dz, &dn);
                f_direct = 0;
                if(dz == gx && dn == gy)
                {    
                    f_direct = 1;
                    
                }
                if(KvadGetHexel(ptr, j + dz, i + dn)->fld == 1 + f_direct)
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

void DirtUpdate(Kvad_t* ptr)
{
    int gx = 0, gy = 1;
    int dz = 0, dn = 0;
    int dz1 = 0, dn1 = 0;
    int dz2 = 0, dn2 = 0;
    int f_direct = 0;
    int b_dir_fall = 0;
    int b_side_fall = 0;
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            
            KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
            if(KvadGetHexel(ptr, j, i)->st8 == 4)
            {
                Force(ptr, j, i, gx, gy, &dz, &dn);
                RelToAbs(ptr, gx, gy, 2, &dz1, &dn1);
                RelToAbs(ptr, gx, gy, -2, &dz2, &dn2);
                b_dir_fall = ((KvadGetHexel(ptr, j + dz1, i + dn1)->st8 != 4)
                             || (KvadGetHexel(ptr, j + dz2, i + dn2)->st8 != 4))
                            && dz == gx && dn == gy;
                b_side_fall = (KvadGetHexel(ptr, j + dz1, i + dn1)->st8 != 4
                            && KvadGetHexel(ptr, j + dz2, i + dn2)->st8 != 4)
                            && (dz != gx || dn != gy);
                f_direct = 0;
                if(b_dir_fall)
                {
                    f_direct = 1;
                    
                    if( KvadGetHexel(ptr, j + dz, i + dn)->fld != -2
                      && KvadGetHexel(ptr, j + dz, i + dn)->mat == 0)
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = 2;
                    }
                    else
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = -2;
                    }
                }
                else if(b_side_fall)
                {
                    if( KvadGetHexel(ptr, j + dz, i + dn)->fld == 0
                      && KvadGetHexel(ptr, j + dz, i + dn)->mat == 0)
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = 1;
                    }
                    else if( KvadGetHexel(ptr, j + dz, i + dn)->fld == 1)
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = -1;
                    }
                }
            }
        }
    }

    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            if(KvadGetHexel(ptr, j, i)->st8 == 4)
            {
                Force(ptr, j, i, gx, gy, &dz, &dn);
                f_direct = 0;
                if(dz == gx && dn == gy)
                {    
                    f_direct = 1;
                    
                }
                if(KvadGetHexel(ptr, j + dz, i + dn)->fld == 1 + f_direct)
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

void RopeUpdate(Kvad_t* ptr)
{
    int gx = 0, gy = 1;
    int dz = 0, dn = 0;
    int dz1 = 0, dn1 = 0;
    int dz2 = 0, dn2 = 0;
    int dz3 = 0, dn3 = 0;
    int dz4 = 0, dn4 = 0;
    int f_direct = 0;
    int b_dir_fall = 0;
    int b_side_fall = 0;
    int b_up_fall = 0;
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            
            
            if(KvadGetHexel(ptr, j, i)->st8 == 5)
            {
                KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
                ForceTension(ptr, j, i, gx, gy, &dz, &dn);
                RelToAbs(ptr, gx, gy, 1, &dz1, &dn1);
                RelToAbs(ptr, gx, gy, -1, &dz2, &dn2);
                RelToAbs(ptr, gx, gy, 2, &dz3, &dn3);
                RelToAbs(ptr, gx, gy, -2, &dz4, &dn4);

                b_up_fall = 
                (
                dz == -gx && dn == -gy
                )
                ;
                b_dir_fall = 
                (
                    (KvadGetHexel(ptr, j + dz1, i + dn1)->st8 == 5) &&
                    (KvadGetHexel(ptr, j + dz2, i + dn2)->st8 == 5) &&
                    !(
                        (KvadGetHexel(ptr, j - gx, i - gy)->st8 == 5) &&
                        (KvadGetHexel(ptr, j + dz3, i + dn3)->st8 == 0) && 
                        (KvadGetHexel(ptr, j + dz4, i + dn4)->st8 == 0)
                    )
                ) || 
                (
                    (KvadGetHexel(ptr, j + dz3, i + dn3)->st8 != 5) && 
                    (KvadGetHexel(ptr, j + dz4, i + dn4)->st8 != 5) && 
                    (KvadGetHexel(ptr, j - gx, i - gy)->st8 != 5)
                ) || 
                (
                    (
                        (KvadGetHexel(ptr, j + dz1, i + dn1)->st8 == 5) &&
                        (KvadGetHexel(ptr, j + dz4, i + dn4)->st8 != 5) &&
                        !(
                            (KvadGetHexel(ptr, j - gx, i - gy)->st8 == 5) &&
                            (KvadGetHexel(ptr, j + dz3, i + dn3)->st8 != 5)
            
                        )
                    ) ||
                    (
                        (KvadGetHexel(ptr, j + dz2, i + dn2)->st8 == 5) && 
                        (KvadGetHexel(ptr, j + dz3, i + dn3)->st8 != 5) &&
                        !(
                            (KvadGetHexel(ptr, j - gx, i - gy)->st8 == 5) &&
                            (KvadGetHexel(ptr, j + dz4, i + dn4)->st8 != 5)
                        )
                    )
                )
                && dz == gx && dn == gy;
                b_side_fall = 
                (
                dz != gx || dn != gy
                )
                ;
                //b_side_fall = 1;
                //b_dir_fall = 0;
                //f_direct = 0;
                if(b_up_fall)
                {
                    f_direct = 2;
                    
                    if( KvadGetHexel(ptr, j + dz, i + dn)->fld != -3
                      && KvadGetHexel(ptr, j + dz, i + dn)->mat == 0)
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = 3;
                    }
                    else
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = -3;
                    }
                }
                else if(b_dir_fall)
                {
                    f_direct = 1;
                    
                    if( (KvadGetHexel(ptr, j + dz, i + dn)->fld != -2 && 
                        KvadGetHexel(ptr, j + dz, i + dn)->fld != -3)
                      && KvadGetHexel(ptr, j + dz, i + dn)->mat == 0)
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = 2;
                    }
                    else
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = -2;
                    }
                }
                else if(b_side_fall)
                {
                    //dz = dz1, dn = dn1;
                    if( KvadGetHexel(ptr, j + dz, i + dn)->fld == 0
                      && KvadGetHexel(ptr, j + dz, i + dn)->mat == 0)
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = 1;
                    }
                    else if( KvadGetHexel(ptr, j + dz, i + dn)->fld == 1)
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = -1;
                    }
                }
            }
        }
    }

    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            if(KvadGetHexel(ptr, j, i)->st8 == 5)
            {
                ForceTension(ptr, j, i, gx, gy, &dz, &dn);
                f_direct = 0;
                if(dz == gx && dn == gy)
                {    
                    f_direct = 1;
                    
                }
                if(dz == -gx && dn == -gy)
                {    
                    f_direct = 2;
                    
                }
                if(KvadGetHexel(ptr, j + dz, i + dn)->fld == 1 + f_direct)
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

void LiquidUpdate(Kvad_t* ptr)
{
    int gx = 0, gy = 1;
    int dz = 0, dn = 0;
    int gout = 0, nout = 0;
    
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            KvadGetHexel(ptr, j, i)->fld2 = 0;
            KvadGetHexel(ptr, j, i)->fld = 0;
        }
    }

    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
            if(KvadGetHexel(ptr, j, i)->st8 == 6)
            {
                for(int di = -1; di < 2; di++)
                {
                    for(int dj = -1; dj < 2; dj++)
                    {
                        if( di * dj != 1)
                            KvadGetHexel(ptr, j + dj, i + di)->fld2 += 1;
                    }
                }
                //KvadGetHexel(ptr, j + 2, i - 1)->fld2 += 3;
                //KvadGetHexel(ptr, j - 2, i + 1)->fld2 += 3;
                for(int di = -2; di < 3; di++)
                {
                    for(int dj = -2; dj < 3; dj++)
                    {
                        if( abs(di + dj) < 3)
                            KvadGetHexel(ptr, j + dj, i + di)->fld2 += 0;
                    }
                }
                for(int di = -3; di < 4; di++)
                {
                    for(int dj = -3; dj < 4; dj++)
                    {
                        if( abs(di + dj) < 4)
                            KvadGetHexel(ptr, j + dj, i + di)->fld2 += 0;
                    }
                }
            }
            else if(KvadGetHexel(ptr, j, i)->st8 != 0)
            {
                KvadGetHexel(ptr, j, i)->fld2 += 2;
            }
        }
    }
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            if(KvadGetHexel(ptr, j, i)->st8 == 6)
            {
                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
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
            if(KvadGetHexel(ptr, j, i)->st8 == 6)
            {
                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
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

void KvadUpdate(Kvad_t* ptr)
{
    Border(ptr);
    SolidUpdate(ptr);
    //WaveUpdate(ptr);
    GasUpdate(ptr);
    SandUpdate(ptr);
    DirtUpdate(ptr);
    RopeUpdate(ptr);
    
    LiquidUpdate(ptr);

    

    //t = mod(t + 1, 12);
}

int NeighbourCount(Kvad_t* ptr, int z, int n, int val)
{
    int N = 0;
    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {
            if( i - j != 0)
                
                N += (KvadGetHexel(ptr, z + j, n + i)->mat == val) ? 1 : 0;
        }
    }
    return N;
}

int HalfNeighCount(Kvad_t* ptr, int z, int n, int val, int dz, int dn)
{
    int dx, dy, dir = 0;

    for(int i = 0; i < 6; i++)
    {
        if(dz == Yrot[i] && dn == Yrot[mod(i - 2, 6)])
        {
            dir = i;
            i = 6;
        }
    }
    dx = dz, dy = dn;
    int front = 0, left = 0, right = 0;
    front = (KvadGetHexel(ptr, z + dx, n + dy)->mat == val) ? 1 : 0;
    
    dx = Yrot[mod(dir + 1    , 6)];
    dy = Yrot[mod(dir + 1 - 2, 6)];
    
    right = (KvadGetHexel(ptr, z + dx, n + dy)->mat == val) ? 1 : 0;
    dx = Yrot[mod(dir - 1    , 6)];
    dy = Yrot[mod(dir - 1 - 2, 6)];
    left = (KvadGetHexel(ptr, z + dx, n + dy)->mat == val) ? 1 : 0;

    return (front + right + left);
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

void Repulsion(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn, int empty_st8)
{
    *dz = 0, *dn = 0;
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if( i - j != 0)
            {
                if(KvadGetHexel(ptr, z + j, n + i)->st8 != empty_st8)
                {
                    *dz -= j;
                    *dn -= i;
                }
            }
        }
    }
    if( *dz == 0 && *dn == 0 )
    {
        *dz = -fx, *dn = -fy;
    }
}

void RelToAbs(Kvad_t* ptr, int fx, int fy, int rel, int* dz, int* dn)
{
    int dir = -1;
    *dz = 0, *dn = 0;
    for(int i = 0; i < 6; i++)
    {
        if(fx == Yrot[i] && fy == Yrot[mod(i - 2, 6)])
        {
            dir = i;
            i = 6;
        }
    }
    if(dir != -1)
    {
        *dz = Yrot[mod(dir + rel    , 6)];
        *dn = Yrot[mod(dir + rel - 2, 6)];
    }
}

void Force(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{
    *dz = 0, *dn = 0;
    int dx, dy, dir = -1;
    dx = fx, dy = fy;

    for(int i = 0; i < 6; i++)
    {
        if(fx == Yrot[i] && fy == Yrot[mod(i - 2, 6)])
        {
            dir = i;
            i = 6;
        }
    }
    int front = 0, left = 0, right = 0;
    front = KvadGetHexel(ptr, z + dx, n + dy)->mat;
    dx = Yrot[mod(dir + 1    , 6)];
    dy = Yrot[mod(dir + 1 - 2, 6)];
    right = KvadGetHexel(ptr, z + dx, n + dy)->mat;
    dx = Yrot[mod(dir - 1    , 6)];
    dy = Yrot[mod(dir - 1 - 2, 6)];
    left = KvadGetHexel(ptr, z + dx, n + dy)->mat;
    
    dx = fx, dy = fy;
    if(front == 0)
    {
        *dz = dx;
        *dn = dy;
    }
    else if( right != left )
    {
        if(right == 0)
        {
            *dz = Yrot[mod(dir + 1    , 6)];
            *dn = Yrot[mod(dir + 1 - 2, 6)];
        }
        
        if(left == 0)
        {
            *dz = Yrot[mod(dir - 1    , 6)];
            *dn = Yrot[mod(dir - 1 - 2, 6)];
        }
    
    }
    
}

void ForceTension(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{
    *dz = 0, *dn = 0;
    int dx, dy, dir = -1;
    dx = fx, dy = fy;

    int left = 0, right = 0;
    int left1 = 0, right1 = 0;

    int rx, ry, lx, ly, r1x, r1y, l1x, l1y;

    for(int i = 0; i < 6; i++)
    {
        if(fx == Yrot[i] && fy == Yrot[mod(i - 2, 6)])
        {
            dir = i;
            i = 6;
        }
    }

    RelToAbs(ptr, fx, fy, 2, &rx, &ry);
    RelToAbs(ptr, fx, fy, -2, &lx, &ly);

    RelToAbs(ptr, fx, fy, 1, &r1x, &r1y);
    RelToAbs(ptr, fx, fy, -1, &l1x, &l1y);

    if
    (
        KvadGetHexel(ptr, z + r1x, n + r1y)->st8 != 5 && 
        KvadGetHexel(ptr, z + l1x, n + l1y)->st8 != 5 && 
        KvadGetHexel(ptr, z + rx, n + ry)->st8 != 5 && 
        KvadGetHexel(ptr, z + lx, n + ly)->st8 != 5 && 
        KvadGetHexel(ptr, z + fx, n + fy)->st8 == 5
    )
    {
        *dz = -fx, *dn = -fy;
    }
    else if(KvadGetHexel(ptr, z + r1x, n + r1y)->st8 == 0 &&
        KvadGetHexel(ptr, z + l1x, n + l1y)->st8 == 0)
    {
        
        
        right = KvadGetHexel(ptr, z + rx, n + ry)->st8;
        left = KvadGetHexel(ptr, z + lx, n + ly)->st8;
        
        dx = fx, dy = fy;
        
        if(right == 5)
        {
            dx += rx, dy += ry;
        }
        if(left == 5)
        {
            dx += lx, dy += ly;
        }

        *dz = dx, *dn = dy;
    }
    else if(KvadGetHexel(ptr, z + fx, n + fy)->st8 != 5)
    {
        *dz = fx, *dn = fy;
    }
    else
    {
        *dz = 0, *dn = 0;
    }
}

void ForceLiquid(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{
    //*dz = fx, *dn = fy;
    *dz = 0, *dn = 0;
    int dir = -1;

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_back = 0, b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0, 
    b_rightback = 0, b_leftback = 0;

    for(int i = 0; i < 6; i++)
    {
        if(fx == Yrot[i] && fy == Yrot[mod(i - 2, 6)])
        {
            dir = i;
            i = 6;
        }
    }

    RelToAbs(ptr, fx, fy, 2, &rbx, &rby);
    RelToAbs(ptr, fx, fy, -2, &lbx, &lby);

    RelToAbs(ptr, fx, fy, 1, &rfx, &rfy);
    RelToAbs(ptr, fx, fy, -1, &lfx, &lfy);

    int neut = 0;

    b_back =
    (
        KvadGetHexel(ptr, z + fx, n + fy)->fld2 > KvadGetHexel(ptr, z, n)->fld2
        || KvadGetHexel(ptr, z - fx, n - fy)->fld2 < KvadGetHexel(ptr, z, n)->fld2
    );
    b_forward =
    (
        KvadGetHexel(ptr, z - fx, n - fy)->fld2 > KvadGetHexel(ptr, z, n)->fld2
        || KvadGetHexel(ptr, z + fx, n + fy)->fld2 < KvadGetHexel(ptr, z, n)->fld2
    );
    b_rightfront =
    (
        KvadGetHexel(ptr, z + lbx, n + lby)->fld2 > KvadGetHexel(ptr, z, n)->fld2
        || KvadGetHexel(ptr, z + rfx, n + rfy)->fld2 < KvadGetHexel(ptr, z, n)->fld2
        
    );
    b_leftfront =
    (
        KvadGetHexel(ptr, z + rbx, n + rby)->fld2 > KvadGetHexel(ptr, z, n)->fld2
        || KvadGetHexel(ptr, z + lfx, n + lfy)->fld2 < KvadGetHexel(ptr, z, n)->fld2
    );
    b_rightback =
    (
        KvadGetHexel(ptr, z + lfx, n + lfy)->fld2 > KvadGetHexel(ptr, z, n)->fld2
        || KvadGetHexel(ptr, z + rbx, n + rby)->fld2 < KvadGetHexel(ptr, z, n)->fld2
    );
    b_leftback =
    (
        KvadGetHexel(ptr, z + rfx, n + rfy)->fld2 > KvadGetHexel(ptr, z, n)->fld2
        || KvadGetHexel(ptr, z + lbx, n + lby)->fld2 < KvadGetHexel(ptr, z, n)->fld2
    );

    //*dz -= fx * b_back, *dn -= fy * b_back;
    //*dz += fx * b_forward, *dn += fy * b_forward;
    //*dz += rfx * b_rightfront, *dn += rfy * b_rightfront;
    //*dz += lfx * b_leftfront, *dn += lfy * b_leftfront;
    //*dz += rbx * b_rightback, *dn += rby * b_rightback;
    //*dz += lbx * b_leftback, *dn += lby * b_leftback;

    if(b_forward || (b_rightfront && b_rightfront)) *dz += fx * b_forward, *dn += fy * b_forward;
    if(b_back || (b_rightback && b_leftback)) *dz -= fx * b_back, *dn -= fy * b_back;
    if(b_rightfront || (b_forward && b_rightback)) *dz += rfx * b_rightfront, *dn += rfy * b_rightfront;
    if(b_leftfront || (b_forward && b_leftback)) *dz += lfx * b_leftfront, *dn += lfy * b_leftfront;
    if(b_rightback || (b_rightfront && b_back)) *dz += rbx * b_rightback, *dn += rby * b_rightback;
    if(b_leftback || (b_leftfront && b_back)) *dz += lbx * b_leftback, *dn += lby * b_leftback;
    if(*dz == 0 && *dn == 0) *dz += fx, *dn += fy;
    //*dz = fx, *dn = fy;
}

int LiquidWaveUpdate(Kvad_t* ptr, int z, int n, int fx, int fy)
{
    KvadGetHexel(ptr, z, n)->tmp = 0;
    if(KvadGetHexel(ptr, z, n)->prs_dist == -1)
    {
        KvadGetHexel(ptr, z, n)->tmp = 0;
    }
    else if(KvadGetHexel(ptr, z, n)->prs_dist == 1)
    {
        KvadGetHexel(ptr, z, n)->tmp = -1;
    }
    else if(KvadGetHexel(ptr, z, n)->prs_dist == 0)
    {
        for(int i = -1; i < 2; i++)
        {
            for(int j = -1; j < 2; j++)
            {
                if( i - j != 0)
                {
                    if(KvadGetHexel(ptr, z + j, n + i)->st8 == 6 && KvadGetHexel(ptr, z + j, n + i)->prs_dist == 1)
                    {
                        KvadGetHexel(ptr, z, n)->tmp = 1;
                    }
                }
            }
        }
    }
}

void Expand(Kvad_t* ptr, int z, int n)
{
    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {
            if( i - j != 0)
            {
                if(KvadGetHexel(ptr, z + j, n + i)->mat == 0)
                {
                    KvadGetHexel(ptr, z + j, n + i)->tmp = 3;
                }
            }
        }
    }
}
/*
b_forward = 
    (
        KvadGetHexel(ptr, z - fx, n - fy)->st8 != 0 ||
        (
            KvadGetHexel(ptr, z + fx, n + fy)->st8 != 6 &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->st8 != 6 &&
            KvadGetHexel(ptr, z + rbx, n + rby)->st8 != 6 &&
            KvadGetHexel(ptr, z - fx, n - fy)->st8 != 6 &&
            KvadGetHexel(ptr, z + lbx, n + lby)->st8 != 6 &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->st8 != 6
            
        )
    );
*/

void PressureIncrease(Kvad_t* ptr, int z, int n, int fx, int fy, int* gout, int* nout)
{
    int dir = -1;
    int gpres_center = KvadGetHexel(ptr, z, n)->prs_grav;
    int npres_center = KvadGetHexel(ptr, z, n)->prs_dist;
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
        
    *gout = 0, *nout = 0;

    int pres_back = 0, pres_forward = 0, 
    pres_rightfront = 0, pres_leftfront = 0, 
    pres_rightback = 0, pres_leftback = 0;

    int dist_back = 0, dist_forward = 0, 
    dist_rightfront = 0, dist_leftfront = 0, 
    dist_rightback = 0, dist_leftback = 0;

    int b_back = 0, b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0, 
    b_rightback = 0, b_leftback = 0;

    for(int i = 0; i < 6; i++)
    {
        if(fx == Yrot[i] && fy == Yrot[mod(i - 2, 6)])
        {
            dir = i;
            i = 6;
        }
    }

    RelToAbs(ptr, fx, fy, 2, &rbx, &rby);
    RelToAbs(ptr, fx, fy, -2, &lbx, &lby);

    RelToAbs(ptr, fx, fy, 1, &rfx, &rfy);
    RelToAbs(ptr, fx, fy, -1, &lfx, &lfy);

    pres_back =         KvadGetHexel(ptr, z - fx, n - fy)->prs_grav + 2;
    dist_back =         KvadGetHexel(ptr, z - fx, n - fy)->prs_dist;
    b_back =            KvadGetHexel(ptr, z - fx, n - fy)->st8 == 6 &&
                        (npres_center >= dist_back ||
                        (npres_center == 0 && dist_back != 0)
                        ) && 
                        gpres_center < pres_back;

    pres_forward =      KvadGetHexel(ptr, z + fx, n + fy)->prs_grav - 2;
    dist_forward =      KvadGetHexel(ptr, z + fx, n + fy)->prs_dist;
    b_forward =         KvadGetHexel(ptr, z + fx, n + fy)->st8 == 6 &&
                        (npres_center >= dist_forward ||
                        (npres_center == 0 && dist_forward != 0)
                        ) && 
                        gpres_center < pres_forward;

    pres_rightfront =   KvadGetHexel(ptr, z + rfx, n + rfy)->prs_grav - 1;
    dist_rightfront =   KvadGetHexel(ptr, z + rfx, n + rfy)->prs_dist;
    b_rightfront =      KvadGetHexel(ptr, z + rfx, n + rfy)->st8 == 6 &&
                        (npres_center >= dist_rightfront ||
                        (npres_center == 0 && dist_rightfront != 0)
                        ) &&
                        gpres_center < pres_rightfront;

    pres_leftfront =    KvadGetHexel(ptr, z + lfx, n + lfy)->prs_grav - 1;
    dist_leftfront =    KvadGetHexel(ptr, z + lfx, n + lfy)->prs_dist;
    b_leftfront =       KvadGetHexel(ptr, z + lfx, n + lfy)->st8 == 6 &&
                        (npres_center >= dist_leftfront ||
                        (npres_center == 0 && dist_leftfront != 0)
                        ) && 
                        gpres_center < pres_leftfront;

    pres_rightback =    KvadGetHexel(ptr, z + rbx, n + rby)->prs_grav + 1;
    dist_rightback =    KvadGetHexel(ptr, z + rbx, n + rby)->prs_dist;
    b_rightback =       KvadGetHexel(ptr, z + rbx, n + rby)->st8 == 6 &&
                        (npres_center >= dist_rightback ||
                        (npres_center == 0 && dist_rightback != 0)
                        ) && 
                        gpres_center < pres_rightback;

    pres_leftback =     KvadGetHexel(ptr, z + lbx, n + lby)->prs_grav + 1;
    dist_leftback =     KvadGetHexel(ptr, z + lbx, n + lby)->prs_dist;
    b_leftback =        KvadGetHexel(ptr, z + lbx, n + lby)->st8 == 6 &&
                        (npres_center >= dist_leftback ||
                        (npres_center == 0 && dist_leftback != 0)
                        ) && 
                        gpres_center < pres_leftback;
    if(b_back)
    {
        gpres_center = pres_back;
        npres_center = dist_back + 1;
    }
    if(b_forward)
    {
        gpres_center = pres_forward;
        npres_center = dist_forward + 1;
    }
    if(b_rightfront)
    {
        gpres_center = pres_rightfront;
        npres_center = dist_rightfront + 1;
    }
    if(b_leftfront)
    {
        gpres_center = pres_leftfront;
        npres_center = dist_leftfront + 1;
    }
    if(b_rightback)
    {
    
        gpres_center = pres_rightback;
        npres_center = dist_rightback + 1;
    }
    if(b_leftback)
    {
        gpres_center = pres_leftback;
        npres_center = dist_leftback + 1;
    }
    
    *gout = gpres_center;
    *nout = npres_center;
}

void Border(Kvad_t* ptr)
{
    for(int i = 0; i < ptr->height; i++)
    {
        KvadSetMat(ptr, 0, i, 8);
        KvadSetMat(ptr, ptr->width - 1, i, 8);
    }
    for(int j = 0; j < ptr->width; j++)
    {
        KvadSetMat(ptr, j, 0, 8);
        KvadSetMat(ptr, j, ptr->height - 1, 8);
    }
}

/*
pres_back =         hmax(KvadGetHexel(ptr, z - fx, n - fy)->prs_grav, 
                            KvadGetHexel(ptr, z - fx, n - fy)->prs_nongrav) + 2 * 1;
    b_back =            KvadGetHexel(ptr, z - fx, n - fy)->st8 == 6;

    pres_forward =      hmax(KvadGetHexel(ptr, z + fx, n + fy)->prs_nongrav, 
                            KvadGetHexel(ptr, z + fx, n + fy)->prs_nongrav) - 2 * 0;
    b_forward =         KvadGetHexel(ptr, z + fx, n + fy)->st8 == 6;

    pres_rightfront =   hmax(KvadGetHexel(ptr, z + rfx, n + rfy)->prs_nongrav, 
                            KvadGetHexel(ptr, z + rfx, n + rfy)->prs_nongrav) - 1 * 0;
    b_rightfront =      KvadGetHexel(ptr, z + rfx, n + rfy)->st8 == 6;

    pres_leftfront =    hmax(KvadGetHexel(ptr, z + lfx, n + lfy)->prs_nongrav, 
                            KvadGetHexel(ptr, z + lfx, n + lfy)->prs_nongrav) - 1 * 0;
    b_leftfront =       KvadGetHexel(ptr, z + lfx, n + lfy)->st8 == 6;

    pres_rightback =    hmax(KvadGetHexel(ptr, z + rbx, n + rby)->prs_grav, 
                            KvadGetHexel(ptr, z + rbx, n + rby)->prs_nongrav) + 1 * 1;
    b_rightback =       KvadGetHexel(ptr, z + rbx, n + rby)->st8 == 6;

    pres_leftback =     hmax(KvadGetHexel(ptr, z + lbx, n + lby)->prs_grav, 
                            KvadGetHexel(ptr, z + lbx, n + lby)->prs_nongrav) + 1 * 1;
    b_leftback =        KvadGetHexel(ptr, z + lbx, n + lby)->st8 == 6;

if(b_back)
    {
        if(npres_center == 0) npres_center = dist_back + 1;
        else
        {
            npres_center = hmin(npres_center, dist_back) + 1;
        }
        gpres_center = pres_back;
    }
    if(b_forward)
    {
        gpres_center = pres_forward;
        if(npres_center == 0) npres_center = dist_forward + 1;
        else
        {
            npres_center = hmin(npres_center, dist_forward) + 1;
        }
        
    }
    if(b_rightfront)
    {
        gpres_center = pres_rightfront;
        if(npres_center == 0) npres_center = dist_rightfront + 1;
        else
        {
            npres_center = hmin(npres_center, dist_rightfront) + 1;
        }
        
    }
    if(b_leftfront)
    {
        gpres_center = pres_leftfront;
        if(npres_center == 0) npres_center = dist_leftfront + 1;
        else
        {
            npres_center = hmin(npres_center, dist_leftfront) + 1;
        }
        
    }
    if(b_rightback)
    {
    
        gpres_center = pres_rightback;
        if(npres_center == 0) npres_center = dist_rightback + 1;
        else
        {
            npres_center = hmin(npres_center, dist_rightback) + 1;
        }
        
    }
    if(b_leftback)
    {
        gpres_center = pres_leftback;
        if(npres_center == 0) npres_center = dist_leftback + 1;
        else
        {
            npres_center = hmin(npres_center, dist_leftback) + 1;
        }
        
    }

( 0 &&
        (
            KvadGetHexel(ptr, z + fx, n + fy)->fld2      == 1 &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->fld2    != 1 &&
            KvadGetHexel(ptr, z + rbx, n + rby)->fld2    != 1 &&
            KvadGetHexel(ptr, z - fx, n - fy)->fld2      != 1 &&
            KvadGetHexel(ptr, z + lbx, n + lby)->fld2    != 1 &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->fld2    != 1 && 0
        ) ||
        (
            KvadGetHexel(ptr, z + fx, n + fy)->st8      == 0 &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->st8    == 0 &&
            KvadGetHexel(ptr, z + rbx, n + rby)->st8    != 0 &&
            KvadGetHexel(ptr, z - fx, n - fy)->st8      != 0 &&
            KvadGetHexel(ptr, z + lbx, n + lby)->st8    != 0 &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->st8    == 0 && 0
        ) ||
        (
            KvadGetHexel(ptr, z + fx, n + fy)->st8      == 0 &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->st8    == 0 &&
            KvadGetHexel(ptr, z + rbx, n + rby)->st8    == 0 &&
            KvadGetHexel(ptr, z - fx, n - fy)->st8      == 0 &&
            KvadGetHexel(ptr, z + lbx, n + lby)->st8    == 0 &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->st8    == 0 && 0
            
        )
    );

*/
