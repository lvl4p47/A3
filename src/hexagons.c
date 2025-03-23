#include "hexagons.h"

const int mat_amount = 9;
int*** p_rules;
int* neighbours_required;

int Yrot[6] = {1, 0, -1, -1, 0, 1};

int t;



Rules_t *RULES;

void RulesInitialize()
{
    /*
    p_rules:
        p_rules[i]                      for mat i
        p_rules[i][j]                   from mat i to mat j (array of pointers to...)
        p_rules[i][j][0]                flag, = -1 disabled, = 0 - enabled
        p_rules[i][j][1...6]            required neighbor mat
        
        examples:
        {1, 1, 2, 3, 4, 5, 6}           enabled, requires neighbors: 1, 2, 3, 4, 5, 6 (not in order)
        
        
    */
    RULES = (Rules_t*)
        malloc(sizeof(Rules_t));
    RULES->frommat = (RulesFromMat_t**)
        malloc(mat_amount * sizeof(RulesFromMat_t*));
    
    for(int i = 0; i < mat_amount; i++)
    {
        RULES->frommat[i] = (RulesFromMat_t*)
            malloc(sizeof(RulesFromMat_t));
        RULES->frommat[i]->mat_from = i;
        RULES->frommat[i]->tomat = (RulesToMat_t**)
            malloc(mat_amount * sizeof(RulesToMat_t*));
        for(int j = 0; j < mat_amount; j++)
        {
            RULES->frommat[i]->tomat[j] = (RulesToMat_t*)
                malloc(sizeof(RulesToMat_t));
            RULES->frommat[i]->tomat[j]->num = 0;
            RULES->frommat[i]->tomat[j]->req = (RulesConditions_t**)
                malloc(1 * sizeof(RulesConditions_t*));
            RulesAdd(i, j, -1, -1, -1, -1, -1, -1, -1);
        }
    }
    
    neighbours_required = malloc(mat_amount * sizeof(int)); 
    
    
    //p_rules = malloc(7 * mat_amount * mat_amount * sizeof(int**));
    // for(int i = 0; i < mat_amount; i++)
    // {
    //     p_rules[i] = malloc(7 * mat_amount * sizeof(int*));
    //     for(int j = 0; j < mat_amount; j++)
    //     {
    //         p_rules[i][j] = malloc(7 * sizeof(int));
    //         for(int k = 0; k < 7; k++)
    //         {
    //             p_rules[i][j][k] = -1;
    //         }
    //     }
    // }
    // neighbours_required = malloc(mat_amount * sizeof(int));    

    RulesCHange(1, 2, 0, 0, 2, 0, -1, -1, -1, -1);
    // p_rules[1][2][0] = 0;
    // p_rules[1][2][1] = 2;
    // p_rules[1][2][2] = 0;

    RulesCHange(2, 0, 0, 0, -1, -1, -1, -1, -1, -1);
    // p_rules[2][0][0] = 0;
    
    RulesCHange(3, 7, 0, 0, 7, 7, 7, 7, 7, -1);
    // p_rules[3][7][0] = 0;
    // p_rules[3][7][1] = 7;
    // p_rules[3][7][2] = 7;
    // p_rules[3][7][3] = 7;
    // p_rules[3][7][4] = 7;
    // p_rules[3][7][5] = 7;

    RulesCHange(3, 6, 0, 0, 2, -1, -1, -1, -1, -1);
    // p_rules[3][6][0] = 0;
    // p_rules[3][6][1] = 2;
    
    RulesCHange(4, 5, 0, 0, 5, 5, 5, 5, -1, -1);
    // p_rules[4][5][0] = 0;
    // p_rules[4][5][1] = 5;
    // p_rules[4][5][2] = 5;
    // p_rules[4][5][3] = 5;
    // p_rules[4][5][4] = 5;

    RulesCHange(5, 4, 0, 0, 5, 5, 5, 3, 3, 0);
    // p_rules[5][4][0] = 0;
    // p_rules[5][4][1] = 5;
    // p_rules[5][4][2] = 5;
    // p_rules[5][4][3] = 5;
    // p_rules[5][4][4] = 3;
    // p_rules[5][4][5] = 3;
    // p_rules[5][4][6] = 0;
    
    RulesCHange(6, 3, 0, 0, 6, 6, 6, 0, 0, 0);
    // p_rules[6][3][0] = 0;
    // p_rules[6][3][1] = 6;
    // p_rules[6][3][2] = 6;
    // p_rules[6][3][3] = 6;
    // p_rules[6][3][4] = 0;
    // p_rules[6][3][5] = 0;
    // p_rules[6][3][6] = 0;
    
    RulesAdd(6, 3, 0, 7, 7, 7, -1, -1, -1);
    // p_rules[6][7][0] = 0;
    // p_rules[6][7][1] = 7;
    // p_rules[6][7][2] = 7;
    // p_rules[6][7][3] = 7;
    
    RulesCHange(7, 3, 0, 0, 2, -1, -1, -1, -1, -1);
    // p_rules[7][3][0] = 0;
    // p_rules[7][3][1] = 2;
    
    RulesCHange(8, 4, 0, 0, 3, 3, 3, -1, -1, -1);
    // p_rules[8][4][0] = 0;
    // p_rules[8][4][1] = 3;
    // p_rules[8][4][2] = 3;
    // p_rules[8][4][3] = 3;
    
}

void RulesTerminate()
{

    RulesConditions_t *p_conds;
    RulesToMat_t **p_tomat;
    RulesFromMat_t **p_frommat;
    for(int i = 0; i < mat_amount; i++)
    {
        for(int j = 0; j < mat_amount; j++)
        {
            while(RULES->frommat[i]->tomat[j]->num > 0)
            {
                free(RULES->frommat[i]->tomat[j]->req[ RULES->frommat[i]->tomat[j]->num - 1 ]);
                RULES->frommat[i]->tomat[j]->num--;
            }
            free(RULES->frommat[i]->tomat[j]->req);
            free(RULES->frommat[i]->tomat[j]);
            
            
        }
        free(RULES->frommat[i]->tomat);
        free(RULES->frommat[i]);
    }
    free(RULES->frommat);
    free(RULES);
}

void RulesAdd(int from, int to, int flag, int n1, int n2, int n3, int n4, int n5, int n6)
{
    int num = RULES->frommat[from]->tomat[to]->num;
    
    RULES->frommat[from]->tomat[to]->req = (RulesConditions_t**)
                realloc(RULES->frommat[from]->tomat[to]->req, (num + 1) * sizeof(RulesConditions_t*));
    
    RULES->frommat[from]->tomat[to]->req[num] = (RulesConditions_t*)
                    malloc(sizeof(RulesConditions_t));
    RULES->frommat[from]->tomat[to]->req[num]->flag = flag;
    RULES->frommat[from]->tomat[to]->req[num]->neighbors[0] = n1;
    RULES->frommat[from]->tomat[to]->req[num]->neighbors[1] = n2;
    RULES->frommat[from]->tomat[to]->req[num]->neighbors[2] = n3;
    RULES->frommat[from]->tomat[to]->req[num]->neighbors[3] = n4;
    RULES->frommat[from]->tomat[to]->req[num]->neighbors[4] = n5;
    RULES->frommat[from]->tomat[to]->req[num]->neighbors[5] = n6;
    RULES->frommat[from]->tomat[to]->num++;
}

void RulesCHange(int from, int to, int num, int flag, int n1, int n2, int n3, int n4, int n5, int n6)
{
    if(num < RULES->frommat[from]->tomat[to]->num && num >= 0)
    {
        RULES->frommat[from]->tomat[to]->req[num]->flag = flag;
        RULES->frommat[from]->tomat[to]->req[num]->neighbors[0] = n1;
        RULES->frommat[from]->tomat[to]->req[num]->neighbors[1] = n2;
        RULES->frommat[from]->tomat[to]->req[num]->neighbors[2] = n3;
        RULES->frommat[from]->tomat[to]->req[num]->neighbors[3] = n4;
        RULES->frommat[from]->tomat[to]->req[num]->neighbors[4] = n5;
        RULES->frommat[from]->tomat[to]->req[num]->neighbors[5] = n6;
    }
}

void RulesRemove(int from, int to, int num)
{
    if(RULES->frommat[from]->tomat[to]->num > 1)
    {
        free(RULES->frommat[from]->tomat[to]->req[ RULES->frommat[from]->tomat[to]->num - 1 ]);
        RULES->frommat[from]->tomat[to]->num--;
    }
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
            ptr->arr[i][j].dns = 0;
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
        cptr->dns = 0;
        cptr->clr = 7;
        break;
    case 1:
        cptr->st8 = 5;
        cptr->dns = 4;
        cptr->clr = 2;
        break;
    case 2:
        cptr->st8 = 2;
        cptr->dns = 4;
        cptr->clr = 1;
        break;
    case 3:
        cptr->st8 = 6;
        cptr->dns = 3;
        cptr->clr = 3;
        break;
    case 4:
        cptr->st8 = 3;
        cptr->dns = 5;
        cptr->clr = 6;
        break;
    case 5:
        cptr->st8 = 4;
        cptr->dns = 5;
        cptr->clr = 1;
        break;
    case 6:
        cptr->st8 = 1;
        cptr->dns = 1;
        cptr->clr = 7;
        break;
    case 7:
        cptr->st8 = 7;
        cptr->dns = 2;
        cptr->clr = 4;
        break;
    case 8:
        cptr->st8 = 2;
        cptr->dns = 6;
        cptr->clr = 7;
    default:
        cptr->st8 = 2;
        cptr->dns = 6;
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
                for(int cond_num = 0; cond_num < RULES->frommat[cen]->tomat[new]->num; cond_num++)
                {
                    if(RULES->frommat[cen]->tomat[new]->req[cond_num]->flag != -1)
                    {
                        for(int i = 0; i < mat_amount; i++)
                        {
                            neighbours_required[i] = 0;
                        }
                        int cur_neighbour_mat;
                        for(int n = 0; n < 6; n++)
                        {
                            if(RULES->frommat[cen]->tomat[new]->req[cond_num]->neighbors[n] != -1)
                            {
                                cur_neighbour_mat = RULES->frommat[cen]->tomat[new]->req[cond_num]->neighbors[n];
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
    int gx = 0, gy = 1;
    int dz = 0, dn = 0;
    int density = 1;
    
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
            if(KvadGetHexel(ptr, j, i)->st8 == 1)
            {
                KvadGetHexel(ptr, j, i)->fld2 += 1;
                for(int di = -1; di < 2; di++)
                {
                    for(int dj = -1; dj < 2; dj++)
                    {
                        if( di * dj != 1)
                            KvadGetHexel(ptr, j + dj, i + di)->fld2 += 1;
                    }
                }
            }
            else if(KvadGetHexel(ptr, j, i)->st8 != 0 
                    && KvadGetHexel(ptr, j, i)->dns >= density)
            {
                KvadGetHexel(ptr, j, i)->fld2 += 2;
            }
        }
    }
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            if(KvadGetHexel(ptr, j, i)->st8 == 1)
            {
                ForceGas(ptr, j, i, -gx, -gy, &dz, &dn);
                if(KvadGetHexel(ptr, j + dz, i + dn)->fld == 0
                 && (KvadGetHexel(ptr, j + dz, i + dn)->dns < KvadGetHexel(ptr, j, i)->dns))
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
                ForceGas(ptr, j, i, -gx, -gy, &dz, &dn);
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
                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                f_direct = 0;
                if(dz == gx && dn == gy)
                {
                    f_direct = 1;
                }
                if(f_direct)
                {
                    if( KvadGetHexel(ptr, j + dz, i + dn)->fld != -2
                      && (KvadGetHexel(ptr, j + dz, i + dn)->dns < KvadGetHexel(ptr, j, i)->dns))
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = 2;
                    }
                    else
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = -2;
                    }
                }
                else
                {
                    if( KvadGetHexel(ptr, j + dz, i + dn)->fld == 0
                      && (KvadGetHexel(ptr, j + dz, i + dn)->dns < KvadGetHexel(ptr, j, i)->dns))
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
                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
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
    int f_direct = 0;
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            
            KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
            if(KvadGetHexel(ptr, j, i)->st8 == 4)
            {
                ForceDirt(ptr, j, i, gx, gy, &dz, &dn);
                f_direct = 0;
                if(dz == gx && dn == gy && 0)
                {
                    f_direct = 1;
                }
                if(f_direct)
                {
                    if( KvadGetHexel(ptr, j + dz, i + dn)->fld != -2
                      && (KvadGetHexel(ptr, j + dz, i + dn)->dns < KvadGetHexel(ptr, j, i)->dns))
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = 2;
                    }
                    else
                    {
                        KvadGetHexel(ptr, j + dz, i + dn)->fld = -2;
                    }
                }
                else
                {
                    if( KvadGetHexel(ptr, j + dz, i + dn)->fld == 0
                      && (KvadGetHexel(ptr, j + dz, i + dn)->dns < KvadGetHexel(ptr, j, i)->dns))
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
                ForceDirt(ptr, j, i, gx, gy, &dz, &dn);
                f_direct = 0;
                if(dz == gx && dn == gy && 0)
                {    
                    f_direct = 1;
                    
                }
                if(KvadGetHexel(ptr, j + dz, i + dn)->fld == 1 + f_direct * 0)
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
    
    int f_direct = 0;
    int b_dir_fall = 0;
    int b_side_fall = 0;
    int b_up_fall = 0;
    
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            //KvadGetHexel(ptr, j, i)->fld = 0;
            //KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
        }
    }
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            if(KvadGetHexel(ptr, j, i)->st8 == 5)
            {
                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                
                b_up_fall = dz == -gx && dn == -gy;
                b_dir_fall = dz == gx && dn == gy;
                b_side_fall = (!b_up_fall) && (!b_dir_fall);
                if(b_up_fall)
                {
                    if( KvadGetHexel(ptr, j + dz, i + dn)->fld != -3
                      && (KvadGetHexel(ptr, j + dz, i + dn)->dns < KvadGetHexel(ptr, j, i)->dns))
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
                    if( (KvadGetHexel(ptr, j + dz, i + dn)->fld != -2 && 
                        KvadGetHexel(ptr, j + dz, i + dn)->fld != -3)
                      && (KvadGetHexel(ptr, j + dz, i + dn)->dns < KvadGetHexel(ptr, j, i)->dns))
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
                      && (KvadGetHexel(ptr, j + dz, i + dn)->dns < KvadGetHexel(ptr, j, i)->dns))
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
                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
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
    int density = 3;
    
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
                KvadGetHexel(ptr, j, i)->fld2 += 1;
                for(int di = -1; di < 2; di++)
                {
                    for(int dj = -1; dj < 2; dj++)
                    {
                        if( di * dj != 1)
                            KvadGetHexel(ptr, j + dj, i + di)->fld2 += 1;
                    }
                }
            }
            else if(KvadGetHexel(ptr, j, i)->st8 != 0 
                    && KvadGetHexel(ptr, j, i)->dns >= density)
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
                if(KvadGetHexel(ptr, j + dz, i + dn)->fld == 0
                 && (KvadGetHexel(ptr, j + dz, i + dn)->dns < KvadGetHexel(ptr, j, i)->dns))
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

void IceUpdate(Kvad_t* ptr)
{
    int gx = 0, gy = 1;
    int dz = 0, dn = 0;
    int f_direct = 0;
    int b_dir_fall = 0;
    int b_side_fall = 0;
    int b_up_fall = 0;
    int volume = 0;
    
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            
            KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
            if(KvadGetHexel(ptr, j, i)->st8 == 7)
            {
                ForceIce(ptr, j, i, gx, gy, &dz, &dn);
                b_up_fall = dz == -gx && dn == -gy;
                b_dir_fall = dz == gx && dn == gy;
                b_side_fall = (!b_up_fall) && (!b_dir_fall);
                if(b_up_fall)
                {
                    if( KvadGetHexel(ptr, j + dz, i + dn)->fld != -3
                      && (KvadGetHexel(ptr, j + dz, i + dn)->dns < KvadGetHexel(ptr, j, i)->dns))
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
                    if( (KvadGetHexel(ptr, j + dz, i + dn)->fld != -2 && 
                        KvadGetHexel(ptr, j + dz, i + dn)->fld != -3)
                      && (KvadGetHexel(ptr, j + dz, i + dn)->dns < KvadGetHexel(ptr, j, i)->dns))
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
                      && (KvadGetHexel(ptr, j + dz, i + dn)->dns < KvadGetHexel(ptr, j, i)->dns))
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
            if(KvadGetHexel(ptr, j, i)->st8 == 7)
            {
                ForceIce(ptr, j, i, gx, gy, &dz, &dn);
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
                    if(f_direct == 0) volume++;
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
    // printf("\nIce volume: %i\t", volume);
    // while(volume > 0)
    // {
    //     printf("cэ");
    //     volume--;
    // }
}

void KvadUpdate(Kvad_t* ptr)
{
    Border(ptr);
    SolidUpdate(ptr);
    DirtUpdate(ptr);
    SandUpdate(ptr);
    IceUpdate(ptr);
    RopeUpdate(ptr);
    GasUpdate(ptr);
    LiquidUpdate(ptr);
    
    //WaveUpdate(ptr);
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

void ForceDirt(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{
    *dz = 0, *dn = 0;
    int dir = -1;

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0;

    int right_sum = 0, left_sum = 0;

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

    right_sum =
    (
        (KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= KvadGetHexel(ptr, z, n)->dns) + 
        (KvadGetHexel(ptr, z + rbx, n + rby)->dns >= KvadGetHexel(ptr, z, n)->dns)
    );
    left_sum =
    (
        (KvadGetHexel(ptr, z + lfx, n + lfy)->dns >= KvadGetHexel(ptr, z, n)->dns) + 
        (KvadGetHexel(ptr, z + lbx, n + lby)->dns >= KvadGetHexel(ptr, z, n)->dns)
    );
    b_forward =
    (
        KvadGetHexel(ptr, z + fx, n + fy)->dns < KvadGetHexel(ptr, z, n)->dns &&
        (
            (
                KvadGetHexel(ptr, z + rbx, n + rby)->dns < KvadGetHexel(ptr, z, n)->dns
            ) ||
            (
                KvadGetHexel(ptr, z + lbx, n + lby)->dns < KvadGetHexel(ptr, z, n)->dns
            )
        ) &&
        (
            NeighbourCount(ptr, z, n, 5) != 3 ||
            abs(right_sum - left_sum) != 1
        ) &&
        !(
            (
                KvadGetHexel(ptr, z - fx, n - fy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
                KvadGetHexel(ptr, z + fx, n + fy)->dns < KvadGetHexel(ptr, z, n)->dns &&
                (
                    (
                        KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
                        KvadGetHexel(ptr, z + lfx, n + lfy)->dns < KvadGetHexel(ptr, z, n)->dns &&
                        KvadGetHexel(ptr, z + rbx, n + rby)->dns >= KvadGetHexel(ptr, z, n)->dns &&
                        KvadGetHexel(ptr, z + lbx, n + lby)->dns < KvadGetHexel(ptr, z, n)->dns
                    ) ||
                    (
                        KvadGetHexel(ptr, z + rfx, n + rfy)->dns < KvadGetHexel(ptr, z, n)->dns &&
                        KvadGetHexel(ptr, z + lfx, n + lfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
                        KvadGetHexel(ptr, z + rbx, n + rby)->dns < KvadGetHexel(ptr, z, n)->dns &&
                        KvadGetHexel(ptr, z + lbx, n + lby)->dns >= KvadGetHexel(ptr, z, n)->dns
                    )
                )
            )
        )
    );
    if(b_forward == 0)
    {
        if(right_sum > left_sum && (right_sum + left_sum < 3)
         && (KvadGetHexel(ptr, z + lfx, n + lfy)->dns < KvadGetHexel(ptr, z, n)->dns)
         && (KvadGetHexel(ptr, z - fx, n - fy)->dns >= KvadGetHexel(ptr, z, n)->dns))
        {
            b_leftfront = 1;
        }
        if(right_sum < left_sum && (right_sum + left_sum < 3)
         && (KvadGetHexel(ptr, z + rfx, n + rfy)->dns < KvadGetHexel(ptr, z, n)->dns)
         && (KvadGetHexel(ptr, z - fx, n - fy)->dns >= KvadGetHexel(ptr, z, n)->dns))
        {
            b_rightfront = 1;
        }
    }
    
    if(b_forward     )  *dz += fx,  *dn += fy;
    if(b_rightfront  )  *dz += rfx, *dn += rfy;
    if(b_leftfront   )  *dz += lfx, *dn += lfy;
}

void ForceSand(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{
    *dz = 0, *dn = 0;
    int dir = -1;

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0;

    int right_sum = 0, left_sum = 0;

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

    right_sum =
    (
        (KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= KvadGetHexel(ptr, z, n)->dns) + 
        (KvadGetHexel(ptr, z + rbx, n + rby)->dns >= KvadGetHexel(ptr, z, n)->dns)
    );
    left_sum =
    (
        (KvadGetHexel(ptr, z + lfx, n + lfy)->dns >= KvadGetHexel(ptr, z, n)->dns) + 
        (KvadGetHexel(ptr, z + lbx, n + lby)->dns >= KvadGetHexel(ptr, z, n)->dns)
    );
    b_forward =
    (
        KvadGetHexel(ptr, z + fx, n + fy)->dns < KvadGetHexel(ptr, z, n)->dns
    );
    if(right_sum > left_sum && (KvadGetHexel(ptr, z + lfx, n + lfy)->dns < KvadGetHexel(ptr, z, n)->dns))
    {
        b_leftfront = 1;
    }
    if(right_sum < left_sum && (KvadGetHexel(ptr, z + rfx, n + rfy)->dns < KvadGetHexel(ptr, z, n)->dns))
    {
        b_rightfront = 1;
    }

    if(b_leftfront || b_rightfront) b_forward = 0;
    
    if(b_forward     )  *dz += fx,  *dn += fy;
    if(b_rightfront  )  *dz += rfx, *dn += rfy;
    if(b_leftfront   )  *dz += lfx, *dn += lfy;
}

void ForceRope(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{
    *dz = 0, *dn = 0;
    int dir = -1;

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_back = 0, b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0;
    
    int state = KvadGetHexel(ptr, z, n)->dns;

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
    
    b_back =
    (
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     < state &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     >= state &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < state &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < state &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   < state &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   < state
        )
    );
    b_forward =
    (
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     < state &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     < state &&
            (
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns   == state &&
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns   == state
                ) ||
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns   == state &&
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < state &&
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns   < state
                ) ||
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < state &&
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns   == state &&
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns   < state
                ) ||
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < state &&
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < state &&
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns   < state &&
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns   < state
                )
            )
        ) ||
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     >= state && 0 &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     < state &&
            (
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= 
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns
                ) &&
                (
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns >=
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns
                ) &&
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns <
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns
                )
            )
        ) ||
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     >= state &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     < state &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   == state &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   == state &&
            (
                KvadGetHexel(ptr, z + rbx, n + rby)->dns   == state &&
                KvadGetHexel(ptr, z + lbx, n + lby)->dns   == state
            )
        )
        
    );
    b_rightfront =
    (
        (
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < state &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < state &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   == state &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   < state
        ) ||
        (
            KvadGetHexel(ptr, z + fx, n + fy)->dns     == state &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < state &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   == state &&
            (
                (
                    KvadGetHexel(ptr, z - fx, n - fy)->dns     < state &&
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns   < state &&
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns   < state
                    
                ) ||
                (
                    KvadGetHexel(ptr, z - fx, n - fy)->dns     == state &&
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns   == state &&
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns   < state && 0
                )
            )
        ) ||
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     == state &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     == state &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < state &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   == state &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   == state &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   == state
        ) ||
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     < state &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     == state &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < state &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   == state &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   < state &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   == state
        )
    );
    b_leftfront =
    (
        (
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < state &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < state &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   < state &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   == state
        ) ||
        (
            KvadGetHexel(ptr, z + fx, n + fy)->dns     == state &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   == state &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < state &&
            (
                (
                    KvadGetHexel(ptr, z - fx, n - fy)->dns     < state &&
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns   < state &&
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns   < state
                ) ||
                (
                    KvadGetHexel(ptr, z - fx, n - fy)->dns     == state &&
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns   < state &&
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns   == state && 0
                )
            )
        ) ||
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     == state &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     == state &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   == state &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < state &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   == state &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   == state
        ) ||
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     < state &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     == state &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   == state &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < state &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   == state &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   < state
        )
    );
    
    if(b_forward     )  *dz += fx,  *dn += fy;
    if(b_back        )  *dz -= fx,  *dn -= fy;
    if(b_rightfront  )  *dz += rfx, *dn += rfy;
    if(b_leftfront   )  *dz += lfx, *dn += lfy;
}

void ForceLiquid(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{
    *dz = 0, *dn = 0;
    int dir = -1;

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_back = 0, b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0, 
    b_rightback = 0, b_leftback = 0;

    int right_sum = 0, left_sum = 0, back_sum = 0, front_sum = 0;

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


    right_sum =
    (
        KvadGetHexel(ptr, z + rfx, n + rfy)->fld2 + 
        KvadGetHexel(ptr, z + rbx, n + rby)->fld2
    );
    left_sum =
    (
        KvadGetHexel(ptr, z + lfx, n + lfy)->fld2 + 
        KvadGetHexel(ptr, z + lbx, n + lby)->fld2
    );
    back_sum =
    (
        KvadGetHexel(ptr, z - fx, n - fy)->fld2 +
        KvadGetHexel(ptr, z + lbx, n + lby)->fld2 + 
        KvadGetHexel(ptr, z + rbx, n + rby)->fld2
    );
    front_sum =
    (
        KvadGetHexel(ptr, z + fx, n + fy)->fld2 +
        KvadGetHexel(ptr, z + rfx, n + rfy)->fld2 + 
        KvadGetHexel(ptr, z + lfx, n + lfy)->fld2
    );
    
    b_back =
    (
        KvadGetHexel(ptr, z, n)->fld2 > 3 &&
        !(KvadGetHexel(ptr, z + fx, n + fy)->fld2 < KvadGetHexel(ptr, z, n)->fld2)
    );
    b_forward =
    (
        KvadGetHexel(ptr, z + fx, n + fy)->fld2 < KvadGetHexel(ptr, z, n)->fld2
    );
    if(right_sum > left_sum)
    {
        if(back_sum >= front_sum || b_forward == 0) b_leftfront = 1;
        else b_leftback = 1;
    } 
    if(right_sum < left_sum)
    {
        if(back_sum >= front_sum || b_forward == 0) b_rightfront = 1;
        else b_rightback = 1;
    }

    if(b_leftback || b_rightback) b_back = 0;
    if(b_leftfront || b_rightfront) b_forward = 0;
    
    if(b_forward     )  *dz += fx,  *dn += fy;
    if(b_back        )  *dz -= fx,  *dn -= fy;
    if(b_rightfront  )  *dz += rfx, *dn += rfy;
    if(b_leftfront   )  *dz += lfx, *dn += lfy;
    if(b_rightback   )  *dz += rbx, *dn += rby;
    if(b_leftback    )  *dz += lbx, *dn += lby;
}

void ForceIce(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{
    *dz = 0, *dn = 0;
    int dir = -1;

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_forward = 0, b_back = 0,
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

    b_back =
    (
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns < KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            (
                (
                    (
                        (KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= KvadGetHexel(ptr, z, n)->dns) ==
                        (KvadGetHexel(ptr, z + lfx, n + lfy)->dns >= KvadGetHexel(ptr, z, n)->dns)
                    ) &&
                    (
                        KvadGetHexel(ptr, z + rbx, n + rby)->dns < KvadGetHexel(ptr, z, n)->dns &&
                        KvadGetHexel(ptr, z + lbx, n + lby)->dns < KvadGetHexel(ptr, z, n)->dns
                    )
                ) ||
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns >= KvadGetHexel(ptr, z, n)->dns &&
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns >= KvadGetHexel(ptr, z, n)->dns
                )
            )
        )
    );
    b_forward =
    (
        (
            KvadGetHexel(ptr, z + fx, n + fy)->dns < KvadGetHexel(ptr, z, n)->dns &&
            (
                (
                    (
                        KvadGetHexel(ptr, z + rfx, n + rfy)->dns < KvadGetHexel(ptr, z, n)->dns ||
                        KvadGetHexel(ptr, z + lfx, n + lfy)->dns < KvadGetHexel(ptr, z, n)->dns
                    ) &&
                    (
                        KvadGetHexel(ptr, z + rbx, n + rby)->dns < KvadGetHexel(ptr, z, n)->dns &&
                        KvadGetHexel(ptr, z + lbx, n + lby)->dns < KvadGetHexel(ptr, z, n)->dns
                    )
                    
                ) ||
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns < KvadGetHexel(ptr, z, n)->dns &&
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns < KvadGetHexel(ptr, z, n)->dns
                )
            )
        ) ||
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns < KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns < KvadGetHexel(ptr, z, n)->dns &&
            (
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns < KvadGetHexel(ptr, z, n)->dns &&
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns >= KvadGetHexel(ptr, z, n)->dns &&
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns < KvadGetHexel(ptr, z, n)->dns
                ) ||
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns < KvadGetHexel(ptr, z, n)->dns &&
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns < KvadGetHexel(ptr, z, n)->dns &&
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns >= KvadGetHexel(ptr, z, n)->dns
                )
            )
        )
    );
    b_rightfront = 
    (
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns < KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns < KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns < KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns < KvadGetHexel(ptr, z, n)->dns
        )
    );
    b_leftfront =
    (
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns < KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns < KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns < KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns < KvadGetHexel(ptr, z, n)->dns
        )
    );
    b_rightback =
    (
        (
            KvadGetHexel(ptr, z + fx, n + fy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns < KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns >= KvadGetHexel(ptr, z, n)->dns
        )
    );
    b_leftback =
    (
        (
            KvadGetHexel(ptr, z + fx, n + fy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns < KvadGetHexel(ptr, z, n)->dns
        )
    );
    
    if(b_back       )  *dz -= fx,  *dn -= fy;
    if(b_forward    )  *dz += fx,  *dn += fy;
    if(b_rightfront )  *dz += rbx, *dn += rby;
    if(b_leftfront  )  *dz += lbx, *dn += lby;
    if(b_rightback  )  *dz += rbx, *dn += rby;
    if(b_leftback   )  *dz += lbx, *dn += lby;
}

void ForceGas(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{
    *dz = 0, *dn = 0;
    int dir = -1;

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_back = 0, b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0, 
    b_rightback = 0, b_leftback = 0;

    int right_sum = 0, left_sum = 0, back_sum = 0, front_sum = 0;

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


    right_sum =
    (
        KvadGetHexel(ptr, z + rfx, n + rfy)->fld2 + 
        KvadGetHexel(ptr, z + rbx, n + rby)->fld2
    );
    left_sum =
    (
        KvadGetHexel(ptr, z + lfx, n + lfy)->fld2 + 
        KvadGetHexel(ptr, z + lbx, n + lby)->fld2
    );
    back_sum =
    (
        KvadGetHexel(ptr, z - fx, n - fy)->fld2 +
        KvadGetHexel(ptr, z + lbx, n + lby)->fld2 + 
        KvadGetHexel(ptr, z + rbx, n + rby)->fld2
    );
    front_sum =
    (
        KvadGetHexel(ptr, z + fx, n + fy)->fld2 +
        KvadGetHexel(ptr, z + rfx, n + rfy)->fld2 + 
        KvadGetHexel(ptr, z + lfx, n + lfy)->fld2
    );
    
    b_back =
    (
        back_sum < front_sum
    );
    b_forward =
    (
        back_sum >= front_sum
    );
    if(right_sum > left_sum)
    {
        if(back_sum > front_sum) b_leftfront = 1;
        else b_leftback = 1;
    } 
    if(right_sum < left_sum)
    {
        if(back_sum > front_sum) b_rightfront = 1;
        else b_rightback = 1;
    }
    if(b_back == 1) b_leftback = 0, b_rightback = 0;
    if(b_leftfront || b_rightfront) b_forward = 0;
    
    if(b_forward     )  *dz += fx,  *dn += fy;
    if(b_back        )  *dz -= fx,  *dn -= fy;
    if(b_rightfront  )  *dz += rfx, *dn += rfy;
    if(b_leftfront   )  *dz += lfx, *dn += lfy;
    if(b_rightback   )  *dz += rbx, *dn += rby;
    if(b_leftback    )  *dz += lbx, *dn += lby;
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
