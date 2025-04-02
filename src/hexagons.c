#include "hexagons.h"

const int mat_amount = 9;
int ***p_rules;
int *neighbours_required;

int Yrot[6] = {1, 0, -1, -1, 0, 1};

int **st8_dns_clr;

int t;

int firevolume;


Rules_t *RULES;

void HexagonsInitialize()
{
    st8_dns_clr = (int**)malloc(mat_amount * sizeof(int*));
    for (int i = 0; i < mat_amount; i++)
	{
		st8_dns_clr[i] = (int*)malloc(3 * sizeof(int));
        st8_dns_clr[i][0] = 2;
        st8_dns_clr[i][1] = 6;
        st8_dns_clr[i][2] = 7;
    }
    {
        st8_dns_clr[0][0] = 0;
        st8_dns_clr[0][1] = 0;
        st8_dns_clr[0][2] = 7;
        
        st8_dns_clr[1][0] = 5;
        st8_dns_clr[1][1] = 2;
        st8_dns_clr[1][2] = 7;
        
        st8_dns_clr[2][0] = 1;
        st8_dns_clr[2][1] = 1;
        st8_dns_clr[2][2] = 1;
        
        st8_dns_clr[3][0] = 6;
        st8_dns_clr[3][1] = 4;
        st8_dns_clr[3][2] = 3;
        
        st8_dns_clr[4][0] = 3;
        st8_dns_clr[4][1] = 5;
        st8_dns_clr[4][2] = 6;
        
        st8_dns_clr[5][0] = 4;
        st8_dns_clr[5][1] = 5;
        st8_dns_clr[5][2] = 1;
        
        st8_dns_clr[6][0] = 1;
        st8_dns_clr[6][1] = 1;
        st8_dns_clr[6][2] = 7;
        
        st8_dns_clr[7][0] = 7;
        st8_dns_clr[7][1] = 3;
        st8_dns_clr[7][2] = 4;
        
        st8_dns_clr[8][0] = 2;
        st8_dns_clr[8][1] = 5;
        st8_dns_clr[8][2] = 7;
    }
    
    t = 0;

    RulesInitialize();
    
    firevolume = 0;
}

void HexagonsTerminate()
{
    
    for (int i = 0; i < mat_amount; i++)
    {
		free(st8_dns_clr[i]);
    }
    free(st8_dns_clr);

    RulesTerminate();
}

void RulesInitialize()
{
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
    
    RulesChange (1, 2, 0, 0, 2, 0, -1, -1, -1, -1);
    RulesChange (2, 0, 0, 0, -4, -1, -1, -1, -1, -1);
    RulesAdd    (2, 0, 0, -2, -1, -1, -1, -1, -1);
    RulesChange (3, 7, 0, 0, 7, 7, 7, 7, 7, -1);
    RulesChange (3, 6, 0, 0, 2, -1, -1, -1, -1, -1);
    RulesChange (4, 5, 0, 0, 5, 5, 5, 5, -1, -1);
    RulesChange (5, 4, 0, 0, 5, 5, 5, 3, 3, 0);
    RulesChange (6, 3, 0, 0, -2, -2, -2, -2, -5, -4);
    RulesAdd    (6, 3, 0, 7, 7, 7, -1, -1, -1);
    RulesChange (7, 3, 0, 0, 2, -1, -1, -1, -1, -1);
    RulesChange (8, 4, 0, 1, 3, 3, 3, 3, 8, 8);
}

void RulesTerminate()
{

    __attribute__((unused)) RulesConditions_t *p_conds;
    __attribute__((unused)) RulesToMat_t **p_tomat;
    __attribute__((unused)) RulesFromMat_t **p_frommat;
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

void RulesChange(int from, int to, int num, int flag, int n1, int n2, int n3, int n4, int n5, int n6)
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

void RulesRemove(int from, int to, __attribute__((unused)) int num)
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
            ptr->arr[i][j].prs_grav = 0;
            ptr->arr[i][j].prs_dist = 0;
            ptr->arr[i][j].fld = 0;
            ptr->arr[i][j].fld2 = 0;
            ptr->arr[i][j].dx = 0;
            ptr->arr[i][j].dy = 0;
            ptr->arr[i][j].st8 = 0;
            ptr->arr[i][j].dns = 0;
            ptr->arr[i][j].stress = 1;
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
        cptr->st8 = st8_dns_clr[0][0];
        cptr->dns = st8_dns_clr[0][1];
        cptr->clr = st8_dns_clr[0][2];
        cptr->stress = 0;
        break;
    case 1:
        cptr->st8 = st8_dns_clr[1][0];
        cptr->dns = st8_dns_clr[1][1];
        cptr->clr = st8_dns_clr[1][2];
        break;
    case 2:
        cptr->st8 = st8_dns_clr[2][0];
        cptr->dns = st8_dns_clr[2][1];
        cptr->clr = st8_dns_clr[2][2];
        break;
    case 3:
        cptr->st8 = st8_dns_clr[3][0];
        cptr->dns = st8_dns_clr[3][1];;
        cptr->clr = st8_dns_clr[3][2];
        break;
    case 4:
        cptr->st8 = st8_dns_clr[4][0];
        cptr->dns = st8_dns_clr[4][1];
        cptr->clr = st8_dns_clr[4][2];
        break;
    case 5:
        cptr->st8 = st8_dns_clr[5][0];
        cptr->dns = st8_dns_clr[5][1];
        cptr->clr = st8_dns_clr[5][2];
        break;
    case 6:
        cptr->st8 = st8_dns_clr[6][0];
        cptr->dns = st8_dns_clr[6][1];
        cptr->clr = st8_dns_clr[6][2];
        break;
    case 7:
        cptr->st8 = st8_dns_clr[7][0];
        cptr->dns = st8_dns_clr[7][1];
        cptr->clr = st8_dns_clr[7][2];
        break;
    case 8:
        cptr->st8 = st8_dns_clr[8][0];
        cptr->dns = st8_dns_clr[8][1];
        cptr->clr = st8_dns_clr[8][2];
        break;
    default:
        cptr->st8 = 2;
        cptr->dns = 6;
        cptr->clr = 5;
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
    
    int cen, flag;
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
                        flag = RULES->frommat[cen]->tomat[new]->req[cond_num]->flag;
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
                                if(cur_neighbour_mat >= 0) neighbours_required[cur_neighbour_mat]++; 
                                if(cur_neighbour_mat <= -2) neighbours_required[-(cur_neighbour_mat + 2)]--; 
                            }
                        }
                        
                        int neighbour_amount;
                        for(int neighbour_mat = 0; neighbour_mat < mat_amount; neighbour_mat++)
                        {
                            neighbour_amount = NeighbourCount(ptr, j, i, neighbour_mat);
                            if( (flag == 0 && 
                                (
                                    (neighbour_amount >= neighbours_required[neighbour_mat] && neighbours_required[neighbour_mat] >=  0 )||
                                    (neighbour_amount < -neighbours_required[neighbour_mat] && neighbours_required[neighbour_mat] <= -1 )
                                )) ||
                                (flag == 1 &&
                                (
                                    (neighbour_amount == neighbours_required[neighbour_mat] && neighbours_required[neighbour_mat] >=  1 )||
                                    (neighbours_required[neighbour_mat] == 0) ||
                                    (neighbour_amount != -neighbours_required[neighbour_mat] && neighbours_required[neighbour_mat] <= -1 )
                                ))
                                )
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

void AudioCount(Kvad_t* ptr, int b_pause)
{
    int curmat;
    int fire = 0;
    if(b_pause == 0)
    {
        for(int i = 0; i < ptr->height; i++)
        {
            for(int j = 0; j < ptr->width; j++)
            {
                curmat = KvadGetHexel(ptr, j, i)->mat;
                switch (curmat)
                {
                case 2:
                    fire++;
                    break;
                
                default:
                    break;
                }
            }
        }
    }
    
    firevolume = (firevolume * 4 + fire) / 5;
}

void AudioUpdate(Kvad_t* ptr, int b_pause)
{
    AudioCount(ptr, b_pause);
    int log_volume = hlog(firevolume, 1.07);
    
    if(firevolume > 0)
    {
        AudioFirePlay();
        AudioFireSetVolume(log_volume);
    }
}

void PhysicsUpdate(Kvad_t* ptr)
{
    int gx = 0, gy = 1;
    int dz = 0, dn = 0;
    __attribute__((unused)) int density = 3;
    int f_direct = 0;
    int b_dir_fall = 0;
    __attribute__((unused)) int b_side_fall = 0;
    int b_up_fall = 0;
    int border = 1;
    int field_value = 0;
    
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
            switch (KvadGetHexel(ptr, j, i)->st8)
            {
            case 0:
            
                break;
            case 1:
            
                KvadGetHexel(ptr, j, i)->fld2 += 1;
                for(int di = -1; di < 2; di++)
                {
                    for(int dj = -1; dj < 2; dj++)
                    {
                        if( di * dj != 1)
                            KvadGetHexel(ptr, j + dj, i + di)->fld2 += 1;
                    }
                }
            
                break;
            case 6:
            
                KvadGetHexel(ptr, j, i)->fld2 += 1;
                for(int di = -1; di < 2; di++)
                {
                    for(int dj = -1; dj < 2; dj++)
                    {
                        if( di * dj != 1)
                            KvadGetHexel(ptr, j + dj, i + di)->fld2 += 1;
                    }
                }
                
                break;
            
            default:
            
                if(KvadGetHexel(ptr, j, i)->dns > 0)
                {
                    KvadGetHexel(ptr, j, i)->fld2 += 1;
                }
                
                break;
            }
        }
    }
    
    for(int i = border; i < ptr->height - border; i++)
    {
        for(int j = border; j < ptr->width - border; j++)
        {
            f_direct = 0;
            switch (KvadGetHexel(ptr, j, i)->st8)
            {
            case 1:
                
                ForceGas(ptr, j, i, -gx, -gy, &dz, &dn);
                
                break;
            case 2:
                
                ForceRock(ptr, j, i, gx, gy, &dz, &dn);
                
                break;
            case 3:
            
                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {
                    f_direct = 1;
                }
            
                break;
            case 4:
                
                ForceDirt(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {
                    f_direct = 1;
                }
                
                break;
            case 5:
                
                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                b_up_fall = dz == -gx && dn == -gy;
                b_dir_fall = dz == gx && dn == gy;
                
                f_direct = b_dir_fall + b_up_fall * 2;
                
                break;
            case 6:
            
                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                
                break;
            case 7:
                
                ForceIce(ptr, j, i, gx, gy, &dz, &dn);
                b_up_fall = dz == -gx && dn == -gy;
                b_dir_fall = dz == gx && dn == gy;
                
                f_direct = b_dir_fall + b_up_fall * 2;
                
                break;
            default:
                break;
            }
            
            field_value = KvadGetHexel(ptr, j, i)->dns + f_direct;
                
            if(KvadGetHexel(ptr, j, i)->st8 != 0)
            {
                if( KvadGetHexel(ptr, j + dz, i + dn)->fld > -field_value
                    && KvadGetHexel(ptr, j + dz, i + dn)->fld < field_value
                    && (KvadGetHexel(ptr, j + dz, i + dn)->dns < KvadGetHexel(ptr, j, i)->dns))
                {
                    KvadGetHexel(ptr, j + dz, i + dn)->fld = field_value;
                }
                else if(KvadGetHexel(ptr, j + dz, i + dn)->fld == field_value)
                {
                    KvadGetHexel(ptr, j + dz, i + dn)->fld = -field_value;
                }
            }
        }
    }
    for(int i = border; i < ptr->height - border; i++)
    {
        for(int j = border; j < ptr->width - border; j++)
        {
            f_direct = 0;
            switch (KvadGetHexel(ptr, j, i)->st8)
            {
            case 1:
                
                ForceGas(ptr, j, i, -gx, -gy, &dz, &dn);
                
                break;
            case 2:
                
                ForceRock(ptr, j, i, gx, gy, &dz, &dn);
                
                break;
            case 3:
                
                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {
                    f_direct = 1;
                }
                
                break;
            case 4:
            
                ForceDirt(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {
                    f_direct = 1;
                }
                
                break;
            case 5:
                
                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {    
                    f_direct = 1;
                    
                }
                if(dz == -gx && dn == -gy)
                {    
                    f_direct = 2;
                    
                }
                
                break;
            case 6:
            
                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                
                break;
            case 7:
                
                ForceIce(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {    
                    f_direct = 1;
                    
                }
                if(dz == -gx && dn == -gy)
                {    
                    f_direct = 2;
                    
                }
                
                break;
            default:
                break;
            }
            
            field_value = KvadGetHexel(ptr, j, i)->dns + f_direct;
            if(KvadGetHexel(ptr, j + dz, i + dn)->fld == field_value &&
                KvadGetHexel(ptr, j, i)->st8 != 0)
            {
                swap(&KvadGetHexel(ptr, j     , i     )->tmp,
                    &KvadGetHexel(ptr, j + dz, i + dn)->tmp);
            }
        }
    }
    
    for(int i = border; i < ptr->height - border; i++)
    {
        for(int j = border; j < ptr->width - border; j++)
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
    PhysicsUpdate(ptr);
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

void RelToAbs(__attribute__((unused)) Kvad_t* ptr, int fx, int fy, int rel, int* dz, int* dn)
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
    __attribute__((unused)) int dir = -1;

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
    __attribute__((unused)) int dir = -1;

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
    __attribute__((unused)) int dir = -1;

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_back = 0, b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0;
    
    int density = KvadGetHexel(ptr, z, n)->dns;

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
            KvadGetHexel(ptr, z - fx, n - fy)->dns     < density &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     >= density &&
            (
            (KvadGetHexel(ptr, z + rfx, n + rfy)->dns   == density) +
            (KvadGetHexel(ptr, z + lfx, n + lfy)->dns   == density) != 1
            ) && 
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   < density &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   < density
        )
    );
    b_forward =
    (
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     < density &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     < density &&
            (
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns   == density &&
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns   == density && 
                    (
                    (KvadGetHexel(ptr, z + rbx, n + rby)->dns   == density) +
                    (KvadGetHexel(ptr, z + lbx, n + lby)->dns   == density) == 0
                    )
                ) ||
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns   == density &&
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < density &&
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns   < density && 1
                ) ||
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < density &&
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns   == density &&
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns   < density && 1
                ) ||
                (
                    KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < density &&
                    KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < density &&
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns   < density &&
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns   < density && 1
                )
            )
        ) ||
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     >= density &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     < density &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   == density &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   == density &&
            (
                KvadGetHexel(ptr, z + rbx, n + rby)->dns   == density &&
                KvadGetHexel(ptr, z + lbx, n + lby)->dns   == density && 1
            )
        )
        
    );
    b_rightfront =
    (
        (
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < density &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < density &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   == density &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   < density && 
            KvadGetHexel(ptr, z - fx, n - fy)->dns     < density
        ) ||
        (
            KvadGetHexel(ptr, z + fx, n + fy)->dns     == density &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < density &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   == density &&
            (
                (
                    KvadGetHexel(ptr, z - fx, n - fy)->dns     < density &&
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns   < density &&
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns   < density && 1
                    
                )
            )
        ) ||
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     < density &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     == density &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < density &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   == density &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   < density &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   == density
        ) ||
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     == density &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     == density &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < density &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   == density &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   == density &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   == density && 1
        )
    );
    b_leftfront =
    (
        (
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < density &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < density &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   < density &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   == density &&
            KvadGetHexel(ptr, z - fx, n - fy)->dns     < density
        ) ||
        (
            KvadGetHexel(ptr, z + fx, n + fy)->dns     == density &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   == density &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < density &&
            (
                (
                    KvadGetHexel(ptr, z - fx, n - fy)->dns     < density &&
                    KvadGetHexel(ptr, z + rbx, n + rby)->dns   < density &&
                    KvadGetHexel(ptr, z + lbx, n + lby)->dns   < density && 1
                )
            )
        ) ||
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     < density &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     == density &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   == density &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < density &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   == density &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   < density
        ) ||
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns     == density &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns     == density &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   == density &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < density &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns   == density &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns   == density && 1
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
    __attribute__((unused)) int dir = -1;

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
    __attribute__((unused)) int dir = -1;

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
    __attribute__((unused)) int dir = -1;

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
        back_sum >= front_sum && right_sum == left_sum
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
    // if(b_back == 1) b_leftback = 0, b_rightback = 0;
    // if(b_leftfront || b_rightfront) b_forward = 0;
    if(b_forward == 1) b_leftfront = 0, b_rightfront = 0;
    if(b_leftback || b_rightback) b_back = 0;
    
    if(b_forward     )  *dz += fx,  *dn += fy;
    if(b_back        )  *dz -= fx,  *dn -= fy;
    if(b_rightfront  )  *dz += rfx, *dn += rfy;
    if(b_leftfront   )  *dz += lfx, *dn += lfy;
    if(b_rightback   )  *dz += rbx, *dn += rby;
    if(b_leftback    )  *dz += lbx, *dn += lby;
}

void ForceRock(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{   
    *dz = 0, *dn = 0;
    __attribute__((unused)) int dir = -1;

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_back = 0, b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0;

    int right_sum = 0, left_sum = 0;
    int state = KvadGetHexel(ptr, z, n)->st8;

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
        (KvadGetHexel(ptr, z + rfx, n + rfy)->st8 == state) + 
        (KvadGetHexel(ptr, z + rbx, n + rby)->st8 == state)
    );
    left_sum =
    (
        (KvadGetHexel(ptr, z + lfx, n + lfy)->st8 == state) + 
        (KvadGetHexel(ptr, z + lbx, n + lby)->st8 == state)
    );
    b_back = 
    (
        (
            KvadGetHexel(ptr, z - fx, n - fy)->dns < KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + fx, n + fy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns >= KvadGetHexel(ptr, z, n)->dns
        )
    );
    b_forward =
    (
        (KvadGetHexel(ptr, z + fx, n + fy)->dns < KvadGetHexel(ptr, z, n)->dns)
    );
    if(b_forward == 0)
    {
        if(right_sum > left_sum && (right_sum + left_sum <= 3)
         && (KvadGetHexel(ptr, z + lfx, n + lfy)->dns < KvadGetHexel(ptr, z, n)->dns)
         && (KvadGetHexel(ptr, z - fx, n - fy)->dns >= KvadGetHexel(ptr, z, n)->dns))
        {
            b_leftfront = 1;
        }
        if(right_sum < left_sum && (right_sum + left_sum <= 3)
         && (KvadGetHexel(ptr, z + rfx, n + rfy)->dns < KvadGetHexel(ptr, z, n)->dns)
         && (KvadGetHexel(ptr, z - fx, n - fy)->dns >= KvadGetHexel(ptr, z, n)->dns))
        {
            b_rightfront = 1;
        }
    }
    
    if(b_back        )  *dz -= fx,  *dn -= fy;
    if(b_forward     )  *dz += fx,  *dn += fy;
    if(b_rightfront  )  *dz += rfx, *dn += rfy;
    if(b_leftfront   )  *dz += lfx, *dn += lfy;
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
