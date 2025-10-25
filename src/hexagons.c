#include "hexagons.h"

#include "time.h"
#include "stdlib.h"

const int mat_amount = 15;
int ***p_rules;
int *neighbours_required;

int Yrot[6] = {1, 0, -1, -1, 0, 1};

int **st8_dns_clr;

int t = 0, gravtime = 10, rt = 0, rulestime = 10;
int timer = 0, maxtime = 2048000;

int gravx, gravy;

int gravcentx = center, gravcenty = center;

int firevolume, icevolume, sandvolume, watervolume, magmavolume, dirtvolume, rockvolume, fabricvolume;

Rules_t *RULES;

Node_t **meatlist;
Brain_t **genomelist;
Contour_t *maincontour;

int contsize = 10;

int meatlistsize = 10240, brainsize = HUGE, blueprintsize = MEDIUM, usagethreshold = 5, usagearmormax = 10, maxhappiness;
int isarmored = 0, happinessrecord, loopsize, active, bestage = 0;
int doprint = 0, doprintdebug = 0, border = 1, physics = 1, corpses = 1, rules = 1, mutations = 1, startingpopulation = 0;
int manual = 0, radial = 1, showchunks = 0, gravfaloff= 0;
int maxenergy = 3000, maxlife = 1000, nonmutationchance = MEDIUM, nonmutationchancebp = MEDIUM, foodenergy = 300, foodorganics = 100;

FILE *file_ptr;

int integer;
char character;

// const int chunkkvadsize = 32;
// int chunksize = 32;
int chunkkvad[chunkkvadsize][chunkkvadsize];

void HexagonsInitialize()
{
    srand(time(NULL));
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
        st8_dns_clr[1][1] = 3;
        st8_dns_clr[1][2] = 7;
        
        st8_dns_clr[2][0] = 1;
        st8_dns_clr[2][1] = 1;
        st8_dns_clr[2][2] = 1;
        
        st8_dns_clr[3][0] = 6;
        st8_dns_clr[3][1] = 2;
        st8_dns_clr[3][2] = 3;
        
        st8_dns_clr[4][0] = 3;
        st8_dns_clr[4][1] = 4;
        st8_dns_clr[4][2] = 6;
        
        st8_dns_clr[5][0] = 4;
        st8_dns_clr[5][1] = 4;
        st8_dns_clr[5][2] = 1;
        
        st8_dns_clr[6][0] = 1;
        st8_dns_clr[6][1] = 1;
        st8_dns_clr[6][2] = 7;
        
        st8_dns_clr[7][0] = 7;
        st8_dns_clr[7][1] = 2;
        st8_dns_clr[7][2] = 4;
        
        st8_dns_clr[8][0] = 2;
        st8_dns_clr[8][1] = 5;
        st8_dns_clr[8][2] = 7;
        
        st8_dns_clr[9][0] = 8;
        st8_dns_clr[9][1] = 4;
        st8_dns_clr[9][2] = 2;
        
        st8_dns_clr[10][0] = 9;
        st8_dns_clr[10][1] = 4;
        st8_dns_clr[10][2] = 5;
        
        st8_dns_clr[11][0] = 10;
        st8_dns_clr[11][1] = 5;
        st8_dns_clr[11][2] = 1;
        
        st8_dns_clr[12][0] = 9;
        st8_dns_clr[12][1] = 4;
        st8_dns_clr[12][2] = 5;
        
        st8_dns_clr[13][0] = 9;
        st8_dns_clr[13][1] = 4;
        st8_dns_clr[13][2] = 5;
        
        st8_dns_clr[14][0] = 7;
        st8_dns_clr[14][1] = 4;
        st8_dns_clr[14][2] = 6;
    }
    
    t = 0;

    RulesInitialize();
    maincontour = ContourListInitialize();
    ChunkKvadZero();
    
    firevolume = 0;
    SetGravity(0, 1);
}

void HexagonsTerminate()
{
    
    for (int i = 0; i < mat_amount; i++)
    {
		free(st8_dns_clr[i]);
    }
    free(st8_dns_clr);

    RulesTerminate();
    
    ContourListTerminate(maincontour);
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
    
    // RulesChange (0, 9, 0, 1, 9, -2, -2, -2, -2, -2);
    RulesChange (0, 9, 0, 0, 9, 5, 3, -1, -1, -1);
    // RulesAdd (0, 9, 0, 9, 9, 9, 9, -1, -1);
    RulesChange (1, 2, 0, 0, 2, 0, -1, -1, -1, -1);
    RulesAdd    (1, 2, 0, 11, -1, -1, -1, -1, -1);
    RulesChange (2, 0, 0, 0, -4, -1, -1, -1, -1, -1);
    RulesAdd    (2, 0, 0, -2, -1, -1, -1, -1, -1);
    RulesChange (3, 7, 0, 1, 7, 0, 0, 0, 0, -1);
    RulesChange (3, 6, 0, 0, 2, -1, -1, -1, -1, -1);
    RulesAdd    (3, 6, 0, 11, 11, -1, -1, -1, -1);
    RulesChange (4, 11, 0, 0, 11, -1, -1, -1, -1, -1);
    RulesChange (4, 8, 0, 0, 8, 8, 8, 8, -6, -1);
    RulesChange (6, 3, 0, 0, -2, -2, -2, -13, -5, -4);
    // RulesAdd    (6, 3, 0, 0, 0, 0, 0, 0, 0);
    RulesAdd    (6, 3, 0, 7, 7, 7, -1, -1, -1);
    RulesChange (7, 3, 0, 0, 2, -1, -1, -1, -1, -1);
    RulesAdd    (7, 3, 0, 3, 3, 3, 3, 3, -1);
    RulesAdd    (7, 3, 0, 11, -1, -1, -1, -1, -1);
    RulesChange (8, 11, 0, -1, 11, 11, 11, 11, -1, -1);
    RulesAdd    (8, 11, 0, 11, 11, 11, -10, -10, -1);
    RulesChange (8, 4, 0, 0, 3, 3, 3, -10, -10, -10);
    RulesAdd    (8, 4, 0, 3, 4, 4, -1, -1, -1);
    RulesChange (9, 2, 0, 0, 2, 0, -1, -1, -1, -1);
    RulesAdd    (9, 2, 0, 11, -1, -1, -1, -1, -1);
    RulesChange (10, 2, 0, 0, 2, 0, -1, -1, -1, -1);
    RulesAdd    (10, 2, 0, 11, -1, -1, -1, -1, -1);
    // RulesChange (10, 5, 0, 0, 4, 4, -1, -1, -1, -1);
    RulesChange (11, 8, 0, 0, 3, -1, -1, -1, -1, -1);
    RulesAdd    (11, 8, 0, 7, 7, 7, -1, -1, -1);
    RulesAdd    (11, 8, 0, -13, -1, -1, -1, -1, -1);
    RulesChange (12, 2, 0, 0, 2, 0, -1, -1, -1, -1);
    RulesAdd    (12, 2, 0, 11, -1, -1, -1, -1, -1);
    // RulesChange (12, 5, 0, 0, 4, 4, -1, -1, -1, -1);
    RulesChange (13, 2, 0, 0, 2, 0, -1, -1, -1, -1);
    RulesAdd    (13, 2, 0, 11, -1, -1, -1, -1, -1);
    // RulesChange (13, 5, 0, 0, 4, 4, -1, -1, -1, -1);
    RulesChange (14, 2, 0, 0, 2, 0, -1, -1, -1, -1);
    RulesAdd    (14, 2, 0, 11, -1, -1, -1, -1, -1);
    
}

void RulesTerminate()
{
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

void RulesRemove(int from, int to)
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
            ptr->arr[i][j].val1 = 0;
            ptr->arr[i][j].val2 = 0;
            ptr->arr[i][j].fld = 0;
            ptr->arr[i][j].fld2 = 0;
            ptr->arr[i][j].dx = 0;
            ptr->arr[i][j].dy = 0;
            ptr->arr[i][j].st8 = 0;
            ptr->arr[i][j].dns = 0;
            ptr->arr[i][j].stress = 0;
            ptr->arr[i][j].clr = 7;
            ptr->arr[i][j].v1t = 0;
            ptr->arr[i][j].v2t = 0;
            ptr->arr[i][j].pwr = 0;
            ptr->arr[i][j].ded = 1;
            ptr->arr[i][j].nrj = 0;
            ptr->arr[i][j].org = 0;
            ptr->arr[i][j].flow[0] = 0;
            ptr->arr[i][j].flow[1] = 0;
            ptr->arr[i][j].flow[2] = 0;
            ptr->arr[i][j].flow[3] = 0;
            ptr->arr[i][j].flow[4] = 0;
            ptr->arr[i][j].flow[5] = 0;
            
            // KvadSetMat(ptr, j, i, 0);
        }
    }
}

void KvadSetMat(Kvad_t* ptr, int z, int n, int value, int new)
{
    Cell_t* cptr;
    cptr = KvadGetHexel(ptr, z, n);

    cptr->mat = value;
    
    cptr->st8 = st8_dns_clr[value][0];
    cptr->dns = st8_dns_clr[value][1];
    cptr->clr = st8_dns_clr[value][2];
    
    if(new)
    {
        switch (cptr->mat)
        {
        case 5:
            cptr->nrj = 0;
            cptr->org = foodorganics;
            break;
        case 9:
            cptr->nrj = foodenergy;
            cptr->org = foodorganics;
            break;
        
        default:
            cptr->nrj = 0;
            cptr->org = 0;
            break;
        }
        
    }
    
}

Cell_t* KvadGetHexel(Kvad_t* ptr, int z, int n)
{
    z = mod(z, ptr->width);
    n = mod(n, ptr->height);

    return &ptr->arr[n][z];
}

void KvadSwapCells(Kvad_t* ptr, int z, int n, int dz, int dn)
{
    int tmat          = KvadGetHexel(ptr, z + dz, n + dn)->mat;
    int ttmp          = KvadGetHexel(ptr, z + dz, n + dn)->tmp;
    int tenergy       = KvadGetHexel(ptr, z + dz, n + dn)->val1;
    int torganics     = KvadGetHexel(ptr, z + dz, n + dn)->val2;
    int tfld          = KvadGetHexel(ptr, z + dz, n + dn)->fld;
    int tfld2         = KvadGetHexel(ptr, z + dz, n + dn)->fld2;
    int tdx           = KvadGetHexel(ptr, z + dz, n + dn)->dx;
    int tdy           = KvadGetHexel(ptr, z + dz, n + dn)->dy;
    int tst8          = KvadGetHexel(ptr, z + dz, n + dn)->st8;
    int tdns          = KvadGetHexel(ptr, z + dz, n + dn)->dns;
    int tstress       = KvadGetHexel(ptr, z + dz, n + dn)->stress;
    int tclr          = KvadGetHexel(ptr, z + dz, n + dn)->clr;
    
    KvadGetHexel(ptr, z + dz, n + dn)->mat          = KvadGetHexel(ptr, z, n)->mat;
    KvadGetHexel(ptr, z + dz, n + dn)->tmp          = KvadGetHexel(ptr, z, n)->tmp;
    KvadGetHexel(ptr, z + dz, n + dn)->val1       = KvadGetHexel(ptr, z, n)->val1;
    KvadGetHexel(ptr, z + dz, n + dn)->val2     = KvadGetHexel(ptr, z, n)->val2;
    KvadGetHexel(ptr, z + dz, n + dn)->fld          = KvadGetHexel(ptr, z, n)->fld;
    KvadGetHexel(ptr, z + dz, n + dn)->fld2         = KvadGetHexel(ptr, z, n)->fld2;
    KvadGetHexel(ptr, z + dz, n + dn)->dx           = KvadGetHexel(ptr, z, n)->dx;
    KvadGetHexel(ptr, z + dz, n + dn)->dy           = KvadGetHexel(ptr, z, n)->dy;
    KvadGetHexel(ptr, z + dz, n + dn)->st8          = KvadGetHexel(ptr, z, n)->st8;
    KvadGetHexel(ptr, z + dz, n + dn)->dns          = KvadGetHexel(ptr, z, n)->dns;
    KvadGetHexel(ptr, z + dz, n + dn)->stress       = KvadGetHexel(ptr, z, n)->stress;
    KvadGetHexel(ptr, z + dz, n + dn)->clr          = KvadGetHexel(ptr, z, n)->clr;
    
    KvadGetHexel(ptr, z, n)->mat        = tmat;
    KvadGetHexel(ptr, z, n)->tmp        = ttmp;
    KvadGetHexel(ptr, z, n)->val1     = tenergy;
    KvadGetHexel(ptr, z, n)->val2   = torganics;
    KvadGetHexel(ptr, z, n)->fld        = tfld;
    KvadGetHexel(ptr, z, n)->fld2       = tfld2;
    KvadGetHexel(ptr, z, n)->dx         = tdx;
    KvadGetHexel(ptr, z, n)->dy         = tdy;
    KvadGetHexel(ptr, z, n)->st8        = tst8;
    KvadGetHexel(ptr, z, n)->dns        = tdns;
    KvadGetHexel(ptr, z, n)->stress     = tstress;
    KvadGetHexel(ptr, z, n)->clr        = tclr;
}

void KvadPartSwapCells(Kvad_t* ptr, int z, int n, int dz, int dn)
{
    int ttmp          = KvadGetHexel(ptr, z + dz, n + dn)->tmp;
    int tenergy       = KvadGetHexel(ptr, z + dz, n + dn)->val1;
    int torganics     = KvadGetHexel(ptr, z + dz, n + dn)->val2;
    int tdx           = KvadGetHexel(ptr, z + dz, n + dn)->dx;
    int tdy           = KvadGetHexel(ptr, z + dz, n + dn)->dy;
    int tmat          = KvadGetHexel(ptr, z + dz, n + dn)->mat;
    int tdns          = KvadGetHexel(ptr, z + dz, n + dn)->dns;
    int tstress       = KvadGetHexel(ptr, z + dz, n + dn)->stress;
    int tclr          = KvadGetHexel(ptr, z + dz, n + dn)->clr;
    
    KvadGetHexel(ptr, z + dz, n + dn)->tmp          = KvadGetHexel(ptr, z, n)->tmp;
    KvadGetHexel(ptr, z + dz, n + dn)->val1       = KvadGetHexel(ptr, z, n)->val1;
    KvadGetHexel(ptr, z + dz, n + dn)->val2     = KvadGetHexel(ptr, z, n)->val2;
    KvadGetHexel(ptr, z + dz, n + dn)->dx           = KvadGetHexel(ptr, z, n)->dx;
    KvadGetHexel(ptr, z + dz, n + dn)->dy           = KvadGetHexel(ptr, z, n)->dy;
    KvadGetHexel(ptr, z + dz, n + dn)->mat          = KvadGetHexel(ptr, z, n)->mat;
    KvadGetHexel(ptr, z + dz, n + dn)->dns          = KvadGetHexel(ptr, z, n)->dns;
    KvadGetHexel(ptr, z + dz, n + dn)->stress       = KvadGetHexel(ptr, z, n)->stress;
    KvadGetHexel(ptr, z + dz, n + dn)->clr          = KvadGetHexel(ptr, z, n)->clr;
    
    KvadGetHexel(ptr, z, n)->tmp        = ttmp;
    KvadGetHexel(ptr, z, n)->val1     = tenergy;
    KvadGetHexel(ptr, z, n)->val2   = torganics;
    KvadGetHexel(ptr, z, n)->dx         = tdx;
    KvadGetHexel(ptr, z, n)->dy         = tdy;
    KvadGetHexel(ptr, z, n)->mat        = tmat;
    KvadGetHexel(ptr, z, n)->dns        = tdns;
    KvadGetHexel(ptr, z, n)->stress     = tstress;
    KvadGetHexel(ptr, z, n)->clr        = tclr;
}

void KvadSetBlob(Kvad_t* ptr, int z, int n, int value, int rad)
{
    switch (value)
    {
    case 12:
        srand(time(NULL) + z * n * rad);
        
        for(int i = 0; i < meatlistsize; i++)
        {
            if(meatlist[i] == NULL && KvadGetHexel(ptr, z, n)->mat == 0)
            {
                genomelist[i] = BrainInitialize(mod(rad, meatlistsize));
                meatlist[i] = MeatInitialize(ptr, z, n, i);
                genomelist[i]->lastx = z;
                i = meatlistsize;
            }
        }
        break;
    
    default:
        for(int i = -rad; i <= rad; i++)
        {
            for(int j = -rad; j <= rad; j++)
            {
                if( i + j >= -rad && i + j <= rad)
                {
                    if(KvadGetHexel(ptr, z + j, n + i)->mat == 0 || value == 0)
                    {
                        
                        KvadSetMat(ptr, z + j, n + i, value, 1);
                        KvadGetHexel(ptr, z + j, n + i)->tmp = KvadGetHexel(ptr, z + j, n + i)->mat;
                        KvadGetHexel(ptr, z + j, n + i)->stress = 0;
                        KvadGetHexel(ptr, z + j, n + i)->flow[0] = 0;
                        KvadGetHexel(ptr, z + j, n + i)->flow[1] = 0;
                        KvadGetHexel(ptr, z + j, n + i)->flow[2] = 0;
                        KvadGetHexel(ptr, z + j, n + i)->flow[3] = 0;
                        KvadGetHexel(ptr, z + j, n + i)->flow[4] = 0;
                        KvadGetHexel(ptr, z + j, n + i)->flow[5] = 0;
                    }
                    if(chunkkvad[mod(hdiv(z + j, chunksize), chunkkvadsize)][mod(hdiv(n + i, chunksize), chunkkvadsize)] == 0)
                        chunkkvad[mod(hdiv(z + j, chunksize), chunkkvadsize)][mod(hdiv(n + i, chunksize), chunkkvadsize)] = 1;
                    
                }
                
            }
        }
        break;
    }
    ChunkActivate(z, n);
}

void WaveUpdate(Kvad_t* ptr)
{

    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->val2;
        }
    }
    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            if(KvadGetHexel(ptr, j, i)->val2 >= 6)
            {
                for(int di = -1; di < 2; di++)
                {
                    for(int dj = -1; dj < 2; dj++)
                    {
                        if( di - dj != 0)
                        {
                            if( KvadGetHexel(ptr, j + dj, i + di)->val2 == 0 || 1)
                            {
                            
                                KvadGetHexel(ptr, j + dj, i + di)->tmp += 1;//KvadGetHexel(ptr, j, i)->val2 - 1;
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
            KvadGetHexel(ptr, j, i)->val2 = KvadGetHexel(ptr, j, i)->tmp;
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
    for(int ci = 0; ci < chunkkvadsize; ci++)
    {
        for(int cj = 0; cj < chunkkvadsize; cj++)
        {
            if(chunkkvad[cj][ci] >= 2)
            {
                for(int i = ci * chunksize; i < (ci + 1) * chunksize; i++)
                {
                    for(int j = cj * chunksize; j < (cj + 1) * chunksize; j++)
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
            }
        }
    }
    for(int ci = 0; ci < chunkkvadsize; ci++)
    {
        for(int cj = 0; cj < chunkkvadsize; cj++)
        {
            if(chunkkvad[cj][ci] >= 2)
            {
                for(int i = ci * chunksize; i < (ci + 1) * chunksize; i++)
                {
                    for(int j = cj * chunksize; j < (cj + 1) * chunksize; j++)
                    {
                        if(KvadGetHexel(ptr, j, i)->tmp != KvadGetHexel(ptr, j, i)->mat)
                        {
                            KvadSetMat(ptr, j, i, KvadGetHexel(ptr, j, i)->tmp, 1);
                            ChunkActivate(j, i);
                        }
                    }
                }
            }
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
                case 3:
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
    int log_volume;
    float volume = 0.50, logscale = 1.02, logshift = 1;
    
    if(firevolume > 0)
    {
        AudioFirePlay();
        log_volume = hlog(firevolume + logshift, logscale) * volume;
        AudioFireSetVolume(log_volume);
    }
    else AudioFireSetVolume(0);
    if(icevolume > 0)
    {
        AudioIcePlay();
        log_volume = hlog(icevolume + logshift, logscale) * volume;
        AudioIceSetVolume(log_volume);
    }
    else AudioIceSetVolume(0);
    if(sandvolume > 0)
    {
        AudioSandPlay();
        log_volume = hlog(sandvolume + logshift, logscale) * volume;
        AudioSandSetVolume(log_volume);
    }
    else AudioSandSetVolume(0);
    if(watervolume > 0)
    {
        AudioWaterPlay();
        log_volume = hlog(watervolume + logshift, logscale) * volume;
        AudioWaterSetVolume(log_volume);
    }
    else AudioWaterSetVolume(0);
    if(magmavolume > 0)
    {
        AudioMagmaPlay();
        log_volume = hlog(magmavolume + logshift, logscale) * volume;
        AudioMagmaSetVolume(log_volume);
    }
    else AudioMagmaSetVolume(0);
    if(dirtvolume > 0)
    {
        AudioDirtPlay();
        log_volume = hlog(dirtvolume + logshift, logscale) * volume;
        AudioDirtSetVolume(log_volume);
    }
    else AudioDirtSetVolume(0);
    if(rockvolume > 0)
    {
        AudioRockPlay();
        log_volume = hlog(rockvolume + logshift, logscale) * volume;
        AudioRockSetVolume(log_volume);
    }
    else AudioRockSetVolume(0);
    if(fabricvolume > 0)
    {
        AudioFabricPlay();
        log_volume = hlog(fabricvolume + logshift, logscale) * volume;
        AudioFabricSetVolume(log_volume);
    }
    else AudioFabricSetVolume(0);
}

void PhysicsUpdate(Kvad_t* ptr)
{
    int gx = 0, gy = 0;
    int dz = 0, dn = 0;
    
    int priority = 0;
    int b_dir_fall = 0;
    int b_up_fall = 0;
    int field_value = 0;
    
    int gdx = 0, gdy = 0;
    
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int curmat;
    int ice = 0;
    int sand = 0;
    int water = 0;
    int magma = 0;
    int dirt = 0;
    int rock = 0;
    int fabric = 0;
    
    int something = 0;
    
    if(showchunks)
    {
        for(int ci = 0; ci < chunkkvadsize; ci++)
        {
            for(int cj = 0; cj < chunkkvadsize; cj++)
            {
                if(chunkkvad[cj][ci] == 0)
                {
                    for(int i = ci * chunksize; i < (ci + 1) * chunksize; i++)
                    {
                        for(int j = cj * chunksize; j < (cj + 1) * chunksize; j++)
                        {
                            KvadGetHexel(ptr, j, i)->clr = 5;
                            
                        }
                    }
                }
                
                if(chunkkvad[cj][ci] == 2)
                {
                    for(int i = ci * chunksize; i < (ci + 1) * chunksize; i++)
                    {
                        for(int j = cj * chunksize; j < (cj + 1) * chunksize; j++)
                        {
                            KvadGetHexel(ptr, j, i)->clr = 2;
                            
                        }
                    }
                }
                
                if(chunkkvad[cj][ci] == 3)
                {
                    for(int i = ci * chunksize; i < (ci + 1) * chunksize; i++)
                    {
                        for(int j = cj * chunksize; j < (cj + 1) * chunksize; j++)
                        {
                            KvadGetHexel(ptr, j, i)->clr = 4;
                            
                        }
                    }
                }
            }
        }
    }
    
    for(int ci = 0; ci < chunkkvadsize; ci++)
        {
            for(int cj = 0; cj < chunkkvadsize; cj++)
            {
                if(chunkkvad[cj][ci] >= 2)
                {
                    for(int i = ci * chunksize; i < (ci + 1) * chunksize; i++)
                    {
                        for(int j = cj * chunksize; j < (cj + 1) * chunksize; j++)
                        {
                            KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
                            KvadSetMat(ptr, j, i, KvadGetHexel(ptr, j, i)->mat, 0);
                            KvadGetHexel(ptr, j, i)->dx = 0;
                            KvadGetHexel(ptr, j, i)->dy = 0;
                            
                        }
                    }
                }
                
            }
        }
    
    for(int ci = 0; ci < chunkkvadsize; ci++)
        {
            for(int cj = 0; cj < chunkkvadsize; cj++)
            {
                if(chunkkvad[cj][ci] == 2)
                {
                    for(int i = ci * chunksize; i < (ci + 1) * chunksize; i++)
                    {
                        for(int j = cj * chunksize; j < (cj + 1) * chunksize; j++)
                        {
                            {
                            KvadGetHexel(ptr, j, i)->fld2 = 0;
                            switch (KvadGetHexel(ptr, j, i)->st8)
                                {
                                case 9:
                                    if(KvadGetHexel(ptr, j, i)->ded)
                                    {
                                        KvadGetHexel(ptr, j, i)->fld = 0;
                                        KvadGetHexel(ptr, j, i)->stress = 0;
                                    }
                                    else
                                    {
                                    KvadGetHexel(ptr, j, i)->fld = -10;
                                    KvadGetHexel(ptr, j, i)->dx = 0;
                                    KvadGetHexel(ptr, j, i)->dy = 0;
                                    }
                                    break;
                                
                                
                                default:
                                    KvadGetHexel(ptr, j, i)->fld = 0;
                                    KvadGetHexel(ptr, j, i)->stress = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
                if(chunkkvad[cj][ci] == 3)
                {
                    for(int i = ci * chunksize; i < (ci + 1) * chunksize; i += chunksize - 1)
                    {
                        for(int j = cj * chunksize; j < (cj + 1) * chunksize; j++)
                        {
                            {
                            KvadGetHexel(ptr, j, i)->fld2 = 0;
                            switch (KvadGetHexel(ptr, j, i)->st8)
                                {
                                case 9:
                                    if(KvadGetHexel(ptr, j, i)->ded)
                                    {
                                        KvadGetHexel(ptr, j, i)->fld = 0;
                                        KvadGetHexel(ptr, j, i)->stress = 0;
                                    }
                                    else
                                    {
                                    KvadGetHexel(ptr, j, i)->fld = -10;
                                    KvadGetHexel(ptr, j, i)->dx = 0;
                                    KvadGetHexel(ptr, j, i)->dy = 0;
                                    }
                                    break;
                                
                                
                                default:
                                    KvadGetHexel(ptr, j, i)->fld = 0;
                                    KvadGetHexel(ptr, j, i)->stress = 0;
                                    break;
                                }
                            }
                        }
                    }
                    
                    for(int j = cj * chunksize; j < (cj + 1) * chunksize; j += chunksize - 1)
                    {
                        for(int i = ci * chunksize + 1; i < (ci + 1) * chunksize - 1; i++)
                        {
                            {
                            KvadGetHexel(ptr, j, i)->fld2 = 0;
                            switch (KvadGetHexel(ptr, j, i)->st8)
                                {
                                case 9:
                                    if(KvadGetHexel(ptr, j, i)->ded)
                                    {
                                        KvadGetHexel(ptr, j, i)->fld = 0;
                                        KvadGetHexel(ptr, j, i)->stress = 0;
                                    }
                                    else
                                    {
                                    KvadGetHexel(ptr, j, i)->fld = -10;
                                    KvadGetHexel(ptr, j, i)->dx = 0;
                                    KvadGetHexel(ptr, j, i)->dy = 0;
                                    }
                                    break;
                                
                                
                                default:
                                    KvadGetHexel(ptr, j, i)->fld = 0;
                                    KvadGetHexel(ptr, j, i)->stress = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    
    for(int ci = 0; ci < chunkkvadsize; ci++)
        {
            for(int cj = 0; cj < chunkkvadsize; cj++)
            {
                if(chunkkvad[cj][ci] >= 2)
                {
                    for(int i = ci * chunksize; i < (ci + 1) * chunksize; i++)
                    {
                        for(int j = cj * chunksize; j < (cj + 1) * chunksize; j++)
                        {
                            gx = KvadGetHexel(ptr, j, i)->dx;
                            gy = KvadGetHexel(ptr, j, i)->dy;
                            
                            if(t == gravtime
                            && hdist(0, 0, gx + GetGravX(ptr, j, i), gy + GetGravY(ptr, j, i)) < 2)
                            {
                                switch (KvadGetHexel(ptr, j, i)->st8)
                                {
                                case 1:
                                    gx -= GetGravX(ptr, j, i);
                                    gy -= GetGravY(ptr, j, i);
                                    break;
                                case 9:
                                    if(KvadGetHexel(ptr, j, i)->ded)
                                    {
                                        gx += GetGravX(ptr, j, i);
                                        gy += GetGravY(ptr, j, i);
                                    }
                                    break;
                                
                                default:
                                    gx = GetGravX(ptr, j, i);
                                    gy = GetGravY(ptr, j, i);
                                    break;
                                }
                                
                                
                            }
                            
                            RelToAbs(gx, gy, 2, &rbx, &rby);
                            RelToAbs(gx, gy, -2, &lbx, &lby);

                            RelToAbs(gx, gy, 1, &rfx, &rfy);
                            RelToAbs(gx, gy, -1, &lfx, &lfy);
                            priority = 0;
                            switch (KvadGetHexel(ptr, j, i)->st8)
                            {
                            case 1:
                                
                                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                                
                                break;
                            case 2:
                                
                                ForceRock(ptr, j, i, gx, gy, &dz, &dn);
                                
                                break;
                            case 3:
                            
                                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                            
                                break;
                            case 4:
                                
                                ForceDirt(ptr, j, i, gx, gy, &dz, &dn);
                                if(dz == gx && dn == gy)
                                {
                                    priority = 1;
                                }
                                
                                break;
                            case 5:
                                
                                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                                b_up_fall = dz == -gx && dn == -gy;
                                b_dir_fall = dz == gx && dn == gy;
                                
                                priority = b_dir_fall + b_up_fall * 2;
                                
                                break;
                            case 6:
                                
                                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                                if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                                {    
                                    priority = 1;
                                }
                                if( (dz == gx && dn == gy) )
                                {    
                                    priority = 2;
                                }
                                
                                break;
                            case 7:
                                
                                ForceIce(ptr, j, i, gx, gy, &dz, &dn);
                                b_up_fall = dz == -gx && dn == -gy;
                                b_dir_fall = dz == gx && dn == gy;
                                
                                priority = b_dir_fall + b_up_fall * 2;
                                
                                break;
                            case 8:
                            
                                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                                if(dz == gx && dn == gy)
                                {    
                                    priority = 1;
                                    
                                }
                                if(dz == -gx && dn == -gy)
                                {    
                                    priority = 2;
                                    
                                }
                                
                                break;
                            case 9:
                            
                                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                                
                                break;
                            case 10:
                            
                                ForceViscous(ptr, j, i, gx, gy, &dz, &dn);
                                b_up_fall = dz == -gx && dn == -gy;
                                b_dir_fall = dz == gx && dn == gy;
                                
                                priority = b_dir_fall + b_up_fall * 2;
                                
                                break;
                            default:
                                dz = gx, dn = gy;
                                break;
                            }
                            
                            if(KvadGetHexel(ptr, j, i)->val2 > 5)
                            {
                                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                                if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                                {    
                                    priority = 1;
                                    
                                }
                                if( (dz == gx && dn == gy) )
                                {    
                                    priority = 2;
                                    
                                }
                            }
                            
                            field_value = KvadGetHexel(ptr, j, i)->dns + priority;
                                
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
                }
        }
    }
    
    for(int ci = 0; ci < chunkkvadsize; ci++)
        {
            for(int cj = 0; cj < chunkkvadsize; cj++)
            {
                if(chunkkvad[cj][ci] >= 2)
                {
                    something = 0;
                    for(int i = ci * chunksize; i < (ci + 1) * chunksize; i++)
                    {
                        for(int j = cj * chunksize; j < (cj + 1) * chunksize; j++)
                        {
                            gx = KvadGetHexel(ptr, j, i)->dx;
                            gy = KvadGetHexel(ptr, j, i)->dy;
                            
                            if(t == gravtime
                            && hdist(0, 0, gx + GetGravX(ptr, j, i), gy + GetGravY(ptr, j, i)) < 2)
                            {
                                switch (KvadGetHexel(ptr, j, i)->st8)
                                {
                                case 1:
                                    gx -= GetGravX(ptr, j, i);
                                    gy -= GetGravY(ptr, j, i);
                                    break;
                                case 9:
                                    if(KvadGetHexel(ptr, j, i)->ded)
                                    {
                                        gx += GetGravX(ptr, j, i);
                                        gy += GetGravY(ptr, j, i);
                                    }
                                    break;
                                
                                default:
                                    gx = GetGravX(ptr, j, i);
                                    gy = GetGravY(ptr, j, i);
                                    break;
                                }
                            }
                            
                            RelToAbs(gx, gy, 2, &rbx, &rby);
                            RelToAbs(gx, gy, -2, &lbx, &lby);

                            RelToAbs(gx, gy, 1, &rfx, &rfy);
                            RelToAbs(gx, gy, -1, &lfx, &lfy);
                            priority = 0;
                            switch (KvadGetHexel(ptr, j, i)->st8)
                            {
                            case 1:
                                
                                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                                // Repulsion(ptr, j, i, &dz, &dn, 0);
                                
                                break;
                            case 2:
                                
                                ForceRock(ptr, j, i, gx, gy, &dz, &dn);
                                
                                break;
                            case 3:
                                
                                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                                
                                break;
                            case 4:
                            
                                ForceDirt(ptr, j, i, gx, gy, &dz, &dn);
                                if(dz == gx && dn == gy)
                                {
                                    priority = 1;
                                }
                                
                                
                                
                                break;
                            case 5:
                                
                                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                                if(dz == gx && dn == gy)
                                {    
                                    priority = 1;
                                    
                                }
                                if(dz == -gx && dn == -gy)
                                {    
                                    priority = 2;
                                    
                                }
                                
                                break;
                            case 6:
                        
                                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                                if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                                {    
                                    priority = 1;
                                    
                                }
                                if( (dz == gx && dn == gy) )
                                {    
                                    priority = 2;
                                    
                                }
                                
                                break;
                            case 7:
                                
                                ForceIce(ptr, j, i, gx, gy, &dz, &dn);
                                if(dz == gx && dn == gy)
                                {    
                                    priority = 1;
                                    
                                }
                                if(dz == -gx && dn == -gy)
                                {    
                                    priority = 2;
                                    
                                }
                                
                                break;
                            case 8:
                            
                                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                                if(dz == gx && dn == gy)
                                {    
                                    priority = 1;
                                    
                                }
                                if(dz == -gx && dn == -gy)
                                {    
                                    priority = 2;
                                    
                                }
                                
                                break;
                            case 9:
                            
                                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                                
                                break;
                            case 10:
                            
                                ForceViscous(ptr, j, i, gx, gy, &dz, &dn);
                                if(dz == gx && dn == gy)
                                {    
                                    priority = 1;
                                    
                                }
                                if(dz == -gx && dn == -gy)
                                {    
                                    priority = 2;
                                    
                                }
                                
                                break;
                            default:
                                dz = gx, dn = gy;
                                break;
                            }
                            
                            if(KvadGetHexel(ptr, j, i)->val2 > 5)
                            {
                                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                                if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                                {    
                                    priority = 1;
                                    
                                }
                                if( (dz == gx && dn == gy) )
                                {    
                                    priority = 2;
                                    
                                }
                            }
                            
                            field_value = KvadGetHexel(ptr, j, i)->dns + priority;
                            if(KvadGetHexel(ptr, j + dz, i + dn)->fld == field_value &&
                                KvadGetHexel(ptr, j, i)->st8 != 0)
                            {
                                if(chunkkvad[cj][ci] == 2)
                                {
                                    swap(&KvadGetHexel(ptr, j     , i     )->tmp,
                                        &KvadGetHexel(ptr, j + dz, i + dn)->tmp);
                                    swap(&KvadGetHexel(ptr, j     , i     )->nrj,
                                        &KvadGetHexel(ptr, j + dz, i + dn)->nrj);
                                    swap(&KvadGetHexel(ptr, j     , i     )->org,
                                        &KvadGetHexel(ptr, j + dz, i + dn)->org);
                                chunkkvad[mod(hdiv(j + dz, chunksize), chunkkvadsize)][mod(hdiv(i + dn, chunksize), chunkkvadsize)] = 2;
                                something = 1;
                                chunkkvad[mod(hdiv(j, chunksize), chunkkvadsize)][mod(hdiv(i, chunksize), chunkkvadsize)] = 2;
                                something = 1;
                                }
                                    
                                curmat = KvadGetHexel(ptr, j, i)->mat;
                                switch (curmat)
                                
                                {
                                case 1:
                                    if (dz != 0 || dn != 0) 
                                        fabric++;
                                    break;
                                case 3:
                                    if (!(dz == gx && dn == gy || dz == -gx && dn == -gy)) 
                                        water++;
                                    break;
                                case 4:
                                    if (!(dz == gx && dn == gy)) 
                                        sand++;
                                    break;
                                case 5:
                                    if (dz != 0 || dn != 0) 
                                        dirt++;
                                    break;
                                case 7:
                                    if (dz == gx && dn == gy) 
                                        ice++;
                                    break;
                                case 8:
                                    if (!(dz == -gx && dn == -gy) && !(dz == 0 && dn == 0)) 
                                        rock++;
                                    break;
                                case 11:
                                    if (!(dz == gx && dn == gy || dz == -gx && dn == -gy)) 
                                        magma++;
                                    break;
                                
                                default:
                                    break;
                                }
                            }
                        }
                    }
                }
        }
    }
    
    icevolume = (icevolume * 4 + ice) / 5;
    sandvolume = (sandvolume * 4 + sand) / 5;
    fabricvolume = (fabricvolume * 4 + fabric) / 5;
    watervolume = (watervolume * 4 + water) / 5;
    dirtvolume = (dirtvolume * 4 + dirt) / 5;
    rockvolume = (rockvolume * 4 + rock) / 5;
    magmavolume = (magmavolume * 4 + magma) / 5;
    int isgrav = 0;
    
    for(int ci = 0; ci < chunkkvadsize; ci++)
        {
            for(int cj = 0; cj < chunkkvadsize; cj++)
            {
                if(chunkkvad[cj][ci] >= 2)
                {
                    something = 0;
                    isgrav = 1;
                    for(int i = ci * chunksize; i < (ci + 1) * chunksize; i++)
                    {
                        for(int j = cj * chunksize; j < (cj + 1) * chunksize; j++)
                        {
                            if(KvadGetHexel(ptr, j, i)->tmp != KvadGetHexel(ptr, j, i)->mat)
                            {
                                if(chunkkvad[cj][ci] == 2)
                                {
                                    KvadSetMat(ptr, j, i, KvadGetHexel(ptr, j, i)->tmp, 0);
                                    ChunkActivate(j, i);
                                }
                                something = 1;
                            }
                            if(GetGravX(ptr, j, i) == 0 && GetGravY(ptr, j, i) == 0)
                            {
                                isgrav = 0;
                                
                            }
            
                            KvadGetHexel(ptr, j, i)->val2 = KvadGetHexel(ptr, j, i)->v2t;
                            KvadGetHexel(ptr, j, i)->v2t = 0;
                        }
                    }
                    if(something == 0 && isgrav == 1 && chunkkvad[cj][ci] == 2) chunkkvad[cj][ci] = 3;
                    else if(something == 0 && isgrav == 1 && chunkkvad[cj][ci] == 3) chunkkvad[cj][ci] = 0;
                }
        }
    }
    
    

}

void PhysicsUpdate2(Kvad_t* ptr)
{
    int gx = 0, gy = 0;
    int dz = 0, dn = 0;
    
    int priority = 0;
    int b_dir_fall = 0;
    int b_up_fall = 0;
    int field_value = 0;
    
    int gdx = 0, gdy = 0;
    
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int curmat;
    int ice = 0;
    int sand = 0;
    int water = 0;
    int magma = 0;
    int dirt = 0;
    int rock = 0;
    int fabric = 0;
    
    Contour_t *concur = maincontour;
    int j, i;
    
    while(concur != NULL)
    {
        j = concur->x, i = concur->y;
        
        KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
        
        concur = concur->next;
    }
    concur = maincontour;
    while(concur != NULL)
    {
        j = concur->x, i = concur->y;
        
        KvadGetHexel(ptr, j, i)->fld2 = 0;
        switch (KvadGetHexel(ptr, j, i)->st8)
            {
            case 9:
                if(KvadGetHexel(ptr, j, i)->ded)
                {
                    KvadGetHexel(ptr, j, i)->fld = 0;
                    KvadGetHexel(ptr, j, i)->stress = 0;
                }
                else
                {
                KvadGetHexel(ptr, j, i)->fld = -10;
                KvadGetHexel(ptr, j, i)->dx = 0;
                KvadGetHexel(ptr, j, i)->dy = 0;
                }
                break;
            
            
            default:
                KvadGetHexel(ptr, j, i)->fld = 0;
                KvadGetHexel(ptr, j, i)->stress = 0;
                break;
            }
        concur = concur->next;
    }
    concur = maincontour;
    while(concur != NULL)
    {
        j = concur->x, i = concur->y;
        
        gx = KvadGetHexel(ptr, j, i)->dx;
        gy = KvadGetHexel(ptr, j, i)->dy;
        
        if(t == gravtime
        && hdist(0, 0, gx + gravx, gy + gravy) < 2)
        {
            switch (KvadGetHexel(ptr, j, i)->st8)
            {
            case 1:
                gx -= gravx;
                gy -= gravy;
                break;
            case 9:
                if(KvadGetHexel(ptr, j, i)->ded)
                {
                    gx += gravx;
                    gy += gravy;
                }
                break;
            
            default:
                gx += gravx;
                gy += gravy;
                break;
            }
            
            
        }
        
        RelToAbs(gx, gy, 2, &rbx, &rby);
        RelToAbs(gx, gy, -2, &lbx, &lby);

        RelToAbs(gx, gy, 1, &rfx, &rfy);
        RelToAbs(gx, gy, -1, &lfx, &lfy);
        priority = 0;
        switch (KvadGetHexel(ptr, j, i)->st8)
        {
        case 1:
            
            ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
            
            break;
        case 2:
            
            ForceRock(ptr, j, i, gx, gy, &dz, &dn);
            
            break;
        case 3:
        
            ForceSand(ptr, j, i, gx, gy, &dz, &dn);
        
            break;
        case 4:
            
            ForceDirt(ptr, j, i, gx, gy, &dz, &dn);
            if(dz == gx && dn == gy)
            {
                priority = 1;
            }
            
            break;
        case 5:
            
            ForceRope(ptr, j, i, gx, gy, &dz, &dn);
            b_up_fall = dz == -gx && dn == -gy;
            b_dir_fall = dz == gx && dn == gy;
            
            priority = b_dir_fall + b_up_fall * 2;
            
            break;
        case 6:
            
            ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
            if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
            {    
                priority = 1;
            }
            if( (dz == gx && dn == gy) )
            {    
                priority = 2;
            }
            
            break;
        case 7:
            
            ForceIce(ptr, j, i, gx, gy, &dz, &dn);
            b_up_fall = dz == -gx && dn == -gy;
            b_dir_fall = dz == gx && dn == gy;
            
            priority = b_dir_fall + b_up_fall * 2;
            
            break;
        case 8:
        
            ForceRope(ptr, j, i, gx, gy, &dz, &dn);
            if(dz == gx && dn == gy)
            {    
                priority = 1;
                
            }
            if(dz == -gx && dn == -gy)
            {    
                priority = 2;
                
            }
            
            break;
        case 9:
        
            ForceSand(ptr, j, i, gx, gy, &dz, &dn);
            
            break;
        case 10:
        
            ForceViscous(ptr, j, i, gx, gy, &dz, &dn);
            b_up_fall = dz == -gx && dn == -gy;
            b_dir_fall = dz == gx && dn == gy;
            
            priority = b_dir_fall + b_up_fall * 2;
            
            break;
        default:
            dz = gx, dn = gy;
            break;
        }
        
        if(KvadGetHexel(ptr, j, i)->val2 > 5)
        {
            ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
            if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
            {    
                priority = 1;
                
            }
            if( (dz == gx && dn == gy) )
            {    
                priority = 2;
                
            }
        }
        
        field_value = KvadGetHexel(ptr, j, i)->dns + priority;
            
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
        concur = concur->next;
    }
    concur = maincontour;
    while(concur != NULL)
        {
            j = concur->x, i = concur->y;
            
            gx = KvadGetHexel(ptr, j, i)->dx;
            gy = KvadGetHexel(ptr, j, i)->dy;
            
            if(t == gravtime
            && hdist(0, 0, gx + gravx, gy + gravy) < 2)
            {
                switch (KvadGetHexel(ptr, j, i)->st8)
                {
                case 1:
                    gx -= gravx;
                    gy -= gravy;
                    break;
                case 9:
                    if(KvadGetHexel(ptr, j, i)->ded)
                    {
                        gx += gravx;
                        gy += gravy;
                    }
                    break;
                
                default:
                    gx += gravx;
                    gy += gravy;
                    break;
                }
            }
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);

            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            priority = 0;
            switch (KvadGetHexel(ptr, j, i)->st8)
            {
            case 1:
                
                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                // Repulsion(ptr, j, i, &dz, &dn, 0);
                
                break;
            case 2:
                
                ForceRock(ptr, j, i, gx, gy, &dz, &dn);
                
                break;
            case 3:
                
                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                
                break;
            case 4:
            
                ForceDirt(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {
                    priority = 1;
                }
                
                
                
                break;
            case 5:
                
                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {    
                    priority = 1;
                    
                }
                if(dz == -gx && dn == -gy)
                {    
                    priority = 2;
                    
                }
                
                break;
            case 6:
        
                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                {    
                    priority = 1;
                    
                }
                if( (dz == gx && dn == gy) )
                {    
                    priority = 2;
                    
                }
                
                break;
            case 7:
                
                ForceIce(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {    
                    priority = 1;
                    
                }
                if(dz == -gx && dn == -gy)
                {    
                    priority = 2;
                    
                }
                
                break;
            case 8:
            
                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {    
                    priority = 1;
                    
                }
                if(dz == -gx && dn == -gy)
                {    
                    priority = 2;
                    
                }
                
                break;
            case 9:
            
                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                
                break;
            case 10:
            
                ForceViscous(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {    
                    priority = 1;
                    
                }
                if(dz == -gx && dn == -gy)
                {    
                    priority = 2;
                    
                }
                
                break;
            default:
                dz = gx, dn = gy;
                break;
            }
            
            if(KvadGetHexel(ptr, j, i)->val2 > 5)
            {
                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                {    
                    priority = 1;
                    
                }
                if( (dz == gx && dn == gy) )
                {    
                    priority = 2;
                    
                }
            }
            
            field_value = KvadGetHexel(ptr, j, i)->dns + priority;
            if(KvadGetHexel(ptr, j + dz, i + dn)->fld == field_value &&
                KvadGetHexel(ptr, j, i)->st8 != 0)
            {
                swap(&KvadGetHexel(ptr, j     , i     )->tmp,
                    &KvadGetHexel(ptr, j + dz, i + dn)->tmp);
                swap(&KvadGetHexel(ptr, j     , i     )->nrj,
                    &KvadGetHexel(ptr, j + dz, i + dn)->nrj);
                    
            }
            concur = concur->next;
        }
    
    
    
    
    concur = maincontour;
    while(concur != NULL)
        {
            j = concur->x, i = concur->y;
            if(concur->new == 0)
            {
                KvadSetBlob(ptr, j, i, KvadGetHexel(ptr, j, i)->tmp, 0);
                
                KvadGetHexel(ptr, j, i)->val2 = KvadGetHexel(ptr, j, i)->v2t;
                KvadGetHexel(ptr, j, i)->v2t = 0;
            }
            concur = concur->next;
        }
    
    

}

void PhysicsUpdate3(Kvad_t* ptr)
{
    int gx = 0, gy = 0;
    int dz = 0, dn = 0;
    
    int priority = 0;
    int b_dir_fall = 0;
    int b_up_fall = 0;
    int field_value = 0;
    
    int gdx = 0, gdy = 0;
    
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int curmat;
    int ice = 0;
    int sand = 0;
    int water = 0;
    int magma = 0;
    int dirt = 0;
    int rock = 0;
    int fabric = 0;
    
    for(int i = 0; i < ptr->height; i++)
        {
            for(int j = 0; j < ptr->width; j++)
            {
                KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
                
            }
        }
    
    if(t == gravtime)
    {
        for(int i = border; i < ptr->height - border; i++)
        {
            for(int j = border; j < ptr->width - border; j++)
            {
                KvadGetHexel(ptr, j, i)->stress = 0;
                
            }
        }
    }
    
    for(int i = border; i < ptr->height - border; i++)
    {
        for(int j = border; j < ptr->width - border; j++)
        {
            KvadGetHexel(ptr, j, i)->fld2 = 0;
            switch (KvadGetHexel(ptr, j, i)->st8)
                {
                case 6:
                    if(t == gravtime)
                    {   
                        if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                        && KvadGetHexel(ptr, j, i)->stress == 0)
                        {
                            ContourPressure(ptr, j, i);
                        }
                    }
                    break;
                case 9:
                    if(KvadGetHexel(ptr, j, i)->ded)
                    {
                        KvadGetHexel(ptr, j, i)->fld = 0;
                        KvadGetHexel(ptr, j, i)->stress = 0;
                    }
                    else
                    {
                    KvadGetHexel(ptr, j, i)->fld = -10;
                    KvadGetHexel(ptr, j, i)->dx = 0;
                    KvadGetHexel(ptr, j, i)->dy = 0;
                    }
                    break;
                
                
                default:
                    KvadGetHexel(ptr, j, i)->fld = 0;
                    KvadGetHexel(ptr, j, i)->stress = 0;
                    break;
                }
        }
    }
    
    for(int i = border; i < ptr->height - border; i++)
    {
        for(int j = border; j < ptr->width - border; j++)
        {
            gx = KvadGetHexel(ptr, j, i)->dx;
            gy = KvadGetHexel(ptr, j, i)->dy;
            
            if(t == gravtime
            && hdist(0, 0, gx + gravx, gy + gravy) < 2)
            {
                switch (KvadGetHexel(ptr, j, i)->st8)
                {
                case 1:
                    gx -= gravx;
                    gy -= gravy;
                    break;
                case 9:
                    if(KvadGetHexel(ptr, j, i)->ded)
                    {
                        gx += gravx;
                        gy += gravy;
                    }
                    break;
                
                default:
                    gx += gravx;
                    gy += gravy;
                    break;
                }
                
                
            }
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);

            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            priority = 0;
            switch (KvadGetHexel(ptr, j, i)->st8)
            {
            case 1:
                
                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                
                break;
            case 2:
                
                ForceRock(ptr, j, i, gx, gy, &dz, &dn);
                
                break;
            case 3:
            
                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
            
                break;
            case 4:
                
                ForceDirt(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {
                    priority = 1;
                }
                
                break;
            case 5:
                
                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                b_up_fall = dz == -gx && dn == -gy;
                b_dir_fall = dz == gx && dn == gy;
                
                priority = b_dir_fall + b_up_fall * 2;
                
                break;
            case 6:
                
                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                {    
                    priority = 1;
                }
                if( (dz == gx && dn == gy) )
                {    
                    priority = 2;
                }
                
                break;
            case 7:
                
                ForceIce(ptr, j, i, gx, gy, &dz, &dn);
                b_up_fall = dz == -gx && dn == -gy;
                b_dir_fall = dz == gx && dn == gy;
                
                priority = b_dir_fall + b_up_fall * 2;
                
                break;
            case 8:
            
                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {    
                    priority = 1;
                    
                }
                if(dz == -gx && dn == -gy)
                {    
                    priority = 2;
                    
                }
                
                break;
            case 9:
            
                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                
                break;
            case 10:
            
                ForceViscous(ptr, j, i, gx, gy, &dz, &dn);
                b_up_fall = dz == -gx && dn == -gy;
                b_dir_fall = dz == gx && dn == gy;
                
                priority = b_dir_fall + b_up_fall * 2;
                
                break;
            default:
                dz = gx, dn = gy;
                break;
            }
            
            if(KvadGetHexel(ptr, j, i)->val2 > 5)
            {
                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                {    
                    priority = 1;
                    
                }
                if( (dz == gx && dn == gy) )
                {    
                    priority = 2;
                    
                }
            }
            
            field_value = KvadGetHexel(ptr, j, i)->dns + priority;
                
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
            gx = KvadGetHexel(ptr, j, i)->dx;
            gy = KvadGetHexel(ptr, j, i)->dy;
            
            if(t == gravtime
            && hdist(0, 0, gx + gravx, gy + gravy) < 2)
            {
                switch (KvadGetHexel(ptr, j, i)->st8)
                {
                case 1:
                    gx -= gravx;
                    gy -= gravy;
                    break;
                case 9:
                    if(KvadGetHexel(ptr, j, i)->ded)
                    {
                        gx += gravx;
                        gy += gravy;
                    }
                    break;
                
                default:
                    gx += gravx;
                    gy += gravy;
                    break;
                }
            }
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);

            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            priority = 0;
            switch (KvadGetHexel(ptr, j, i)->st8)
            {
            case 1:
                
                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                // Repulsion(ptr, j, i, &dz, &dn, 0);
                
                break;
            case 2:
                
                ForceRock(ptr, j, i, gx, gy, &dz, &dn);
                
                break;
            case 3:
                
                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                
                break;
            case 4:
            
                ForceDirt(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {
                    priority = 1;
                }
                
                
                
                break;
            case 5:
                
                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {    
                    priority = 1;
                    
                }
                if(dz == -gx && dn == -gy)
                {    
                    priority = 2;
                    
                }
                
                break;
            case 6:
        
                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                {    
                    priority = 1;
                    
                }
                if( (dz == gx && dn == gy) )
                {    
                    priority = 2;
                    
                }
                
                break;
            case 7:
                
                ForceIce(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {    
                    priority = 1;
                    
                }
                if(dz == -gx && dn == -gy)
                {    
                    priority = 2;
                    
                }
                
                break;
            case 8:
            
                ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {    
                    priority = 1;
                    
                }
                if(dz == -gx && dn == -gy)
                {    
                    priority = 2;
                    
                }
                
                break;
            case 9:
            
                ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                
                break;
            case 10:
            
                ForceViscous(ptr, j, i, gx, gy, &dz, &dn);
                if(dz == gx && dn == gy)
                {    
                    priority = 1;
                    
                }
                if(dz == -gx && dn == -gy)
                {    
                    priority = 2;
                    
                }
                
                break;
            default:
                dz = gx, dn = gy;
                break;
            }
            
            if(KvadGetHexel(ptr, j, i)->val2 > 5)
            {
                ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                {    
                    priority = 1;
                    
                }
                if( (dz == gx && dn == gy) )
                {    
                    priority = 2;
                    
                }
            }
            
            field_value = KvadGetHexel(ptr, j, i)->dns + priority;
            if(KvadGetHexel(ptr, j + dz, i + dn)->fld == field_value &&
                KvadGetHexel(ptr, j, i)->st8 != 0)
            {
                swap(&KvadGetHexel(ptr, j     , i     )->tmp,
                    &KvadGetHexel(ptr, j + dz, i + dn)->tmp);
                swap(&KvadGetHexel(ptr, j     , i     )->nrj,
                    &KvadGetHexel(ptr, j + dz, i + dn)->nrj);
                    
                curmat = KvadGetHexel(ptr, j, i)->mat;
                switch (curmat)
                
                {
                case 1:
                    if (dz != 0 || dn != 0) 
                        fabric++;
                    break;
                case 3:
                    if (!(dz == gx && dn == gy || dz == -gx && dn == -gy)) 
                        water++;
                    break;
                case 4:
                    if (!(dz == gx && dn == gy)) 
                        sand++;
                    break;
                case 5:
                    if (dz != 0 || dn != 0) 
                        dirt++;
                    break;
                case 7:
                    if (dz == gx && dn == gy) 
                        ice++;
                    break;
                case 8:
                    if (!(dz == -gx && dn == -gy) && !(dz == 0 && dn == 0)) 
                        rock++;
                    break;
                case 11:
                    if (!(dz == gx && dn == gy || dz == -gx && dn == -gy)) 
                        magma++;
                    break;
                
                default:
                    break;
                }
            }
        }
    }
    
    icevolume = (icevolume * 4 + ice) / 5;
    sandvolume = (sandvolume * 4 + sand) / 5;
    fabricvolume = (fabricvolume * 4 + fabric) / 5;
    watervolume = (watervolume * 4 + water) / 5;
    dirtvolume = (dirtvolume * 4 + dirt) / 5;
    rockvolume = (rockvolume * 4 + rock) / 5;
    magmavolume = (magmavolume * 4 + magma) / 5;
    
    
    for(int i = border; i < ptr->height - border; i++)
    {
        for(int j = border; j < ptr->width - border; j++)
        {
            KvadGetHexel(ptr, j, i)->val1 = 
            KvadGetHexel(ptr, j, i)->dx + KvadGetHexel(ptr, j, i)->dy * 3;
            KvadGetHexel(ptr, j, i)->val2 = 
            hmax(KvadGetHexel(ptr, j, i)->val2, 0);
            KvadGetHexel(ptr, j, i)->dx = 0;
            KvadGetHexel(ptr, j, i)->dy = 0;
        }
    }
    
    for(int i = border; i < ptr->height - border; i++)
    {
        for(int j = border; j < ptr->width - border; j++)
        {
            
            gx = mod(KvadGetHexel(ptr, j, i)->val1 + 1, 3) - 1;
            gy = hdiv(KvadGetHexel(ptr, j, i)->val1 + 1, 3);
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);

            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            if(KvadGetHexel(ptr, j, i)->val1 != 0
                    )
            {
                if(KvadGetHexel(ptr, j, i)->val2 > 0)
                {
                    int windstrength = KvadGetHexel(ptr, j, i)->val2;
                    
                    if(KvadGetHexel(ptr, j + gx, i + gy)->dns > 4
                    && (KvadGetHexel(ptr, j + rfx, i + rfy)->dx == 0
                    && KvadGetHexel(ptr, j + rfx, i + rfy)->dy == 0))
                    {
                        if(KvadGetHexel(ptr, j + rfx, i + rfy)->dns <= 4)
                        {
                            KvadGetHexel(ptr, j + rfx, i + rfy)->dx = rfx;
                            KvadGetHexel(ptr, j + rfx, i + rfy)->dy = rfy;
                            
                            KvadGetHexel(ptr, j + rfx, i + rfy)->v2t = 
                            hmax(KvadGetHexel(ptr, j, i)->val2 - 1,
                            KvadGetHexel(ptr, j + rfx, i + rfy)->v2t);
                        }
                        
                        if(KvadGetHexel(ptr, j + lfx, i + lfy)->dns <= 4)
                        {
                            KvadGetHexel(ptr, j + lfx, i + lfy)->dx = lfx;
                            KvadGetHexel(ptr, j + lfx, i + lfy)->dy = lfy;
                            
                            KvadGetHexel(ptr, j + lfx, i + lfy)->v2t = 
                            hmax(KvadGetHexel(ptr, j, i)->val2 - 1,
                            KvadGetHexel(ptr, j + lfx, i + lfy)->v2t);
                        }
                        
                    }
                    
                    
                    
                    else if((KvadGetHexel(ptr, j + rfx, i + rfy)->dx == 0
                    && KvadGetHexel(ptr, j + rfx, i + rfy)->dy == 0))
                    {
                        
                        
                        if(KvadGetHexel(ptr, j + lfx, i + lfy)->dns >= 0)
                        {
                            KvadGetHexel(ptr, j + rfx, i + rfy)->dx = gx;
                            KvadGetHexel(ptr, j + rfx, i + rfy)->dy = gy;
                            
                            KvadGetHexel(ptr, j + rfx, i + rfy)->v2t = 
                            hmax(KvadGetHexel(ptr, j, i)->val2 / 2,
                            KvadGetHexel(ptr, j + rfx, i + rfy)->v2t);
                        
                            KvadGetHexel(ptr, j + lfx, i + lfy)->dx = gx;
                            KvadGetHexel(ptr, j + lfx, i + lfy)->dy = gy;
                            
                            KvadGetHexel(ptr, j + lfx, i + lfy)->v2t = 
                            hmax(KvadGetHexel(ptr, j, i)->val2 / 2,
                            KvadGetHexel(ptr, j + lfx, i + lfy)->v2t);
                        }
                    }
                    KvadGetHexel(ptr, j + gx, i + gy)->dx = gx;
                    KvadGetHexel(ptr, j + gx, i + gy)->dy = gy;
                    KvadGetHexel(ptr, j + gx, i + gy)->v2t = 
                    hmax(KvadGetHexel(ptr, j, i)->val2, 
                    KvadGetHexel(ptr, j + gx, i + gy)->v2t);
                }
                
            }
        }
    }
    
    for(int i = border; i < ptr->height - border; i++)
    {
        for(int j = border; j < ptr->width - border; j++)
        {
            KvadSetMat(ptr, j, i, KvadGetHexel(ptr, j, i)->tmp, 0);
            
            KvadGetHexel(ptr, j, i)->val2 = KvadGetHexel(ptr, j, i)->v2t;
            KvadGetHexel(ptr, j, i)->v2t = 0;
        }
    }
    
    

}

void PositionsUpdate(Kvad_t* ptr)
{
    int gx = 0, gy = 0;
    int dz = 0, dn = 0;
    int border = 1;
    int priority = 0;
    int b_dir_fall = 0;
    int b_up_fall = 0;
    int field_value = 0;
    
    int gdx = 0, gdy = 0;
    
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int curmat;
    int ice = 0;
    int sand = 0;
    int water = 0;
    int magma = 0;
    int dirt = 0;
    int rock = 0;
    int fabric = 0;
    
    int watercounter = 0;
    
    if(t == gravtime)
    {
        for(int i = border; i < ptr->height - border; i++)
        {
            for(int j = border; j < ptr->width - border; j++)
            {
                KvadGetHexel(ptr, j, i)->stress = 0;
                KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
                // KvadGetHexel(ptr, j, i)->clr = 7;
                
            }
        }
    }
    
    if(t == gravtime)
    {
        for(int i = border; i < ptr->height - border; i++)
        {
            for(int j = border; j < ptr->width - border; j++)
            {
                KvadGetHexel(ptr, j, i)->fld2 = 0;
                switch (KvadGetHexel(ptr, j, i)->st8)
                {
                
                case 3:
                    if(t == gravtime)
                    {
                        KvadGetHexel(ptr, j, i)->fld = -10;
                        
                        if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                        && KvadGetHexel(ptr, j, i)->stress == 0)
                        {
                            ContourPressure(ptr, j, i);
                        }
                    }
                    
                    break;
                case 4:
                    if(t == gravtime)
                    {
                        KvadGetHexel(ptr, j, i)->fld = -10;
                        
                        if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                        && KvadGetHexel(ptr, j, i)->stress == 0)
                        {
                            ContourPressure(ptr, j, i);
                        }
                    }
                    
                    break;
                case 6:
                    if(t == gravtime)
                    {
                        KvadGetHexel(ptr, j, i)->fld = -10;
                        
                        if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                        && KvadGetHexel(ptr, j, i)->stress == 0)
                        {
                            ContourPressure(ptr, j, i);
                        }
                    }
                    watercounter++;
                    break;
                case 10:
                    if(t == gravtime)
                    {
                        KvadGetHexel(ptr, j, i)->fld = -10;
                        
                        if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                        && KvadGetHexel(ptr, j, i)->stress == 0)
                        {
                            ContourPressure(ptr, j, i);
                        }
                    }
                    break;
                case 8:
                    KvadGetHexel(ptr, j, i)->fld = -10;
                    KvadGetHexel(ptr, j, i)->dx = 0;
                    KvadGetHexel(ptr, j, i)->dy = 0;
                    break;
                case 9:
                    KvadGetHexel(ptr, j, i)->fld = -10;
                    KvadGetHexel(ptr, j, i)->dx = 0;
                    KvadGetHexel(ptr, j, i)->dy = 0;
                    break;
                
                default:
                    KvadGetHexel(ptr, j, i)->fld = 0;
                    KvadGetHexel(ptr, j, i)->stress = 0;
                    break;
                }
            }
        }
        
        if(doprint) printf("\nwater: %i\n", watercounter);
    }
    
    
    
    if(t == gravtime)
    {
        for(int i = border; i < ptr->height - border; i++)
        {
            for(int j = border; j < ptr->width - border; j++)
            {
                KvadGetHexel(ptr, j, i)->fld2 = 0;
                switch (KvadGetHexel(ptr, j, i)->st8)
                {
                case 3:
                    
                    if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                    && KvadGetHexel(ptr, j, i)->stress >= 0)
                    {
                        ContourMoveSand(ptr, j, i);
                    }
                    break;
                case 4:
                    
                    if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                    && KvadGetHexel(ptr, j, i)->stress >= 0)
                    {
                        ContourMoveSolid(ptr, j, i);
                    }
                    break;
                case 6:
                    
                    if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                    && KvadGetHexel(ptr, j, i)->stress >= 0)
                    {
                        ContourMove(ptr, j, i);
                    }
                    break;
                case 10:
                    
                    if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                    && KvadGetHexel(ptr, j, i)->stress >= 0)
                    {
                        ContourMove(ptr, j, i);
                    }
                    break;
                
                default:
                    
                    break;
                }
            }
        }
    }
    
    if(t == gravtime)
    {
        for(int i = border; i < ptr->height - border; i++)
        {
            for(int j = border; j < ptr->width - border; j++)
            {
                KvadSetMat(ptr, j, i, KvadGetHexel(ptr, j, i)->tmp, 0);
                
                KvadGetHexel(ptr, j, i)->val2 = KvadGetHexel(ptr, j, i)->v2t;
                KvadGetHexel(ptr, j, i)->v2t = 0;
            }
        }
    }
    

}

void PositionsUpdate2(Kvad_t* ptr)
{
    int gx = 0, gy = 0;
    int dz = 0, dn = 0;
    int border = 1;
    int priority = 0;
    int b_dir_fall = 0;
    int b_up_fall = 0;
    int field_value = 0;
    
    int gdx = 0, gdy = 0;
    
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int curmat;
    int ice = 0;
    int sand = 0;
    int water = 0;
    int magma = 0;
    int dirt = 0;
    int rock = 0;
    int fabric = 0;
    
    int watercounter = 0;
    
    Contour_t *concur = maincontour;
    int j, i;
    
    if(t == gravtime)
    {
        while(concur != NULL)
        {
            j = concur->x, i = concur->y;
            KvadGetHexel(ptr, j, i)->stress = 0;
            KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
            // KvadGetHexel(ptr, j, i)->clr = 7;
            
            concur = concur->next;
        }
    }
    concur = maincontour;
    if(t == gravtime)
    {
        while(concur != NULL)
        {
            j = concur->x, i = concur->y;
            
            if(KvadGetHexel(ptr, j, i)->stress == 0)
            {
            
            KvadGetHexel(ptr, j, i)->fld2 = 0;
            switch (KvadGetHexel(ptr, j, i)->st8)
            {
            
            case 3:
                if(t == gravtime)
                {
                    KvadGetHexel(ptr, j, i)->fld = -10;
                    
                    if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                    && KvadGetHexel(ptr, j, i)->stress == 0)
                    {
                        ContourPressure(ptr, j, i);
                    }
                }
                
                break;
            case 4:
                if(t == gravtime)
                {
                    KvadGetHexel(ptr, j, i)->fld = -10;
                    
                    if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                    && KvadGetHexel(ptr, j, i)->stress == 0)
                    {
                        ContourPressure(ptr, j, i);
                    }
                }
                
                break;
            case 6:
                if(t == gravtime)
                {
                    KvadGetHexel(ptr, j, i)->fld = -10;
                    
                    if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                    && KvadGetHexel(ptr, j, i)->stress == 0)
                    {
                        ContourPressure(ptr, j, i);
                    }
                }
                watercounter++;
                break;
            case 10:
                if(t == gravtime)
                {
                    KvadGetHexel(ptr, j, i)->fld = -10;
                    
                    if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                    && KvadGetHexel(ptr, j, i)->stress == 0)
                    {
                        ContourPressure(ptr, j, i);
                    }
                }
                break;
            case 8:
                KvadGetHexel(ptr, j, i)->fld = -10;
                KvadGetHexel(ptr, j, i)->dx = 0;
                KvadGetHexel(ptr, j, i)->dy = 0;
                break;
            case 9:
                KvadGetHexel(ptr, j, i)->fld = -10;
                KvadGetHexel(ptr, j, i)->dx = 0;
                KvadGetHexel(ptr, j, i)->dy = 0;
                break;
            
            default:
                KvadGetHexel(ptr, j, i)->fld = 0;
                KvadGetHexel(ptr, j, i)->stress = 0;
                break;
            }
            }
            concur = concur->next;
        }
        
        if(doprint) printf("\nwater: %i\n", watercounter);
    }
    
    
    concur = maincontour;
    if(t == gravtime)
    {
        while(concur != NULL)
        {
            j = concur->x, i = concur->y;
            
            if(KvadGetHexel(ptr, j, i)->stress >= 0)
            {
            
            KvadGetHexel(ptr, j, i)->fld2 = 0;
            switch (KvadGetHexel(ptr, j, i)->st8)
            {
            case 3:
                
                if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                && KvadGetHexel(ptr, j, i)->stress >= 0)
                {
                    ContourMoveSand(ptr, j, i);
                }
                break;
            case 4:
                
                if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                && KvadGetHexel(ptr, j, i)->stress >= 0)
                {
                    ContourMoveSolid(ptr, j, i);
                }
                break;
            case 6:
                
                if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                && KvadGetHexel(ptr, j, i)->stress >= 0)
                {
                    ContourMove(ptr, j, i);
                }
                break;
            case 10:
                
                if(NeighbourCount(ptr, j, i, KvadGetHexel(ptr, j, i)->mat) < 6
                && KvadGetHexel(ptr, j, i)->stress >= 0)
                {
                    ContourMove(ptr, j, i);
                }
                break;
            
            default:
                
                break;
            }
            }
            concur = concur->next;
        }
    }
    concur = maincontour;
    if(t == gravtime)
    {
        while(concur != NULL)
        {
            j = concur->x, i = concur->y;
            KvadSetMat(ptr, j, i, KvadGetHexel(ptr, j, i)->tmp, 0);
            
            KvadGetHexel(ptr, j, i)->val2 = KvadGetHexel(ptr, j, i)->v2t;
            KvadGetHexel(ptr, j, i)->v2t = 0;
            concur = concur->next;
        }
    }
    

}

void KvadUpdate(Kvad_t* ptr)
{
    if(border) Border(ptr);
    if(rt == rulestime)
    {
        if(rules) SolidUpdate(ptr);
        
        rt = 0;
    }
    rt++;
    
    // PositionsUpdate(ptr);
    
    EntityCollision(ptr, e1);
    MeatListUpdate(ptr);
    // ContourCheck(ptr);
    // ContourPrint(ptr);
    
    // KvadSetBlob(ptr, gravcentx, gravcenty, 8, 0);
    
    // printf("%i %i\n", GetGravX(ptr, 98, 102), GetGravY(ptr, 98, 102));
    
    if(t == gravtime)
    {
        if(physics) 
        {
            
            
            ChunkKvadUpdate();
            // ChunkKvadPrint();
            PhysicsUpdate(ptr);
            // AudioUpdate(ptr, 0);
            
        }
        
        // if(physics) PositionsUpdate2(ptr);
        
        t = 0;
    }
    t++;
    
    if(timer == maxtime)
    {
        timer = 0;
    }
    timer++;
}

void EntityCollision(Kvad_t* ptr, Entity_t* p_e)
{
    int gx = 0, gy = 0;
    
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    gx = GetGravX(ptr, p_e->z, p_e->n);
    gy = GetGravY(ptr, p_e->z, p_e->n);
    
    if(gx == 0 && gy == 0)
    {
        gx = Yrot[mod(p_e->angle, 6)];
        gy = Yrot[mod(p_e->angle - 2, 6)];
    }
    else
    {
        for (int i = 0; i < 6; i++)
        {
            p_e->angle = i;
            if(Yrot[mod(i, 6)] == gx && Yrot[mod(i - 2, 6)] == gy) i = 6;
        }
    }
    
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);

    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);

    if(meatlist[0] != NULL)
    {
        p_e->z = meatlist[0]->x;
        p_e->n = meatlist[0]->y;
    }
    else
    {
    
        int oldz, oldn, oldsubz, oldsubn;
        int newz, newn;
        int max_fuel = 99;
        int fuelused = 0;
        oldz = p_e->z, oldn = p_e->n;
        oldsubz = p_e->subz, oldsubn = p_e->subn;
        
        inpst.vx = (inpst.right - inpst.left);
        inpst.vy = (inpst.down - inpst.up);
        
        p_e->z += hdiv(p_e->subz, p_e->magn);
        p_e->n += hdiv(p_e->subn, p_e->magn);
        
        p_e->subz = mod(p_e->subz, p_e->magn);
        p_e->subn = mod(p_e->subn, p_e->magn);
        
        int dz = 0, dn = 0;
        
        if(inpst.vy < 0 && p_e->fuel > 1)
        {
            dz += -gx, dn += -gy;
            if(t == gravtime && physics) p_e->fuel -= 2, fuelused = 1;
        }
        if(inpst.vy > 0)
            dz += gx, dn += gy;
        else if(inpst.vx > 0)
        {
            dz += lfx, dn += lfy;
        }
        else if(inpst.vx < 0)
        {
            dz += rfx, dn += rfy;
        }
        
        if(!(GetGravX(ptr, p_e->z, p_e->n) == 0 && GetGravY(ptr, p_e->z, p_e->n) == 0)
         && dz == 0 && dn == 0
         && KvadGetHexel(ptr, oldz, oldn)->dns < 2
         && (t == gravtime && physics))
        {
            dz = gx, dn = gy;
            if(inpst.vy >= 0 || 1)
                p_e->fuel = hmin(p_e->fuel + 1, max_fuel);
        }
            
        p_e->subz += dz, p_e->subn += dn;
        
        int s = p_e->magn, s1 = s, s2 = 2 * s;
        if(mod(s , 3) == 0) s1++, s2++;
        int conds[5] = {
            p_e->subz + 2 * p_e->subn < s1,
            2 * p_e->subz + p_e->subn <= s1,
            p_e->subz - p_e->subn < 0,
            p_e->subz + 2 * p_e->subn < s2,
            2 * p_e->subz + p_e->subn <= s2
        };
        
        if      (conds[0] == 1 && conds[1] == 1);
        else if (conds[2] == 0 && conds[3] == 1) 
            p_e->z = p_e->z + 1, p_e->subz = p_e->subz - p_e->magn;
        else if (conds[4] == 1) 
            p_e->n = p_e->n + 1, p_e->subn = p_e->subn - p_e->magn;
        else    
            p_e->z = p_e->z + 1, p_e->subz = p_e->subz - p_e->magn, 
            p_e->n = p_e->n + 1, p_e->subn = p_e->subn - p_e->magn;
            
        newz = p_e->z;
        newn = p_e->n;
        
        
        
        if(KvadGetHexel(ptr, p_e->z, p_e->n)->dns > 4)
        {
            p_e->z = oldz, p_e->n = oldn;
            p_e->subz = oldsubz, p_e->subn = oldsubn;
            
            if(inpst.vy >= 0 && inpst.insertA == 0)
                p_e->fuel = max_fuel;
        }
        else if(KvadGetHexel(ptr, p_e->z, p_e->n)->dns > 1)
        {
            p_e->fuel = hmin(p_e->fuel + 3, max_fuel);//+1
        }
        
        // if(KvadGetHexel(ptr, oldz, oldn)->dns > 4)
        //     p_e->fuel = max_fuel;
        
        int blowpower = 3;
        
        if(inpst.delete == 1)
            KvadSetBlob(ptr, newz, newn, 0, 0);
        if(inpst.insertB)
        {
            KvadGetHexel(ptr, newz, newn)->flow[0] = KvadGetHexel(ptr, newz, newn)->flow[0] + 1;
            KvadGetHexel(ptr, newz, newn)->dx = -dz;
            KvadGetHexel(ptr, newz, newn)->dy = -dn;
            
            KvadGetHexel(ptr, newz, newn)->val1 = 
            KvadGetHexel(ptr, newz, newn)->dx + KvadGetHexel(ptr, newz, newn)->dy * 3;
            
            KvadGetHexel(ptr, oldz, oldn)->dx = -dz;
            KvadGetHexel(ptr, oldz, oldn)->dy = -dn;
            
            KvadGetHexel(ptr, oldz, oldn)->val1 = 
            KvadGetHexel(ptr, oldz, oldn)->dx + KvadGetHexel(ptr, oldz, oldn)->dy * 3;
            
            KvadGetHexel(ptr, oldz, oldn)->val2 = blowpower;
        }
        if(inpst.insertA)
        {
            if(newz != oldz || newn != oldn || inpst.vy >= 0)
            {
                KvadGetHexel(ptr, newz, newn)->stress = KvadGetHexel(ptr, newz, newn)->stress + 1;
                KvadGetHexel(ptr, newz, newn)->dx = dz;
                KvadGetHexel(ptr, newz, newn)->dy = dn;
                
                KvadGetHexel(ptr, newz, newn)->val1 = 
                KvadGetHexel(ptr, newz, newn)->dx + KvadGetHexel(ptr, newz, newn)->dy * 3;
                
                KvadGetHexel(ptr, newz, newn)->val2 = blowpower;
            }
            
            if(inpst.vy < 0)
            {
                KvadGetHexel(ptr, oldz, oldn)->dx = dz;
                KvadGetHexel(ptr, oldz, oldn)->dy = dn;
                
                KvadGetHexel(ptr, oldz, oldn)->val1 = 
                KvadGetHexel(ptr, oldz, oldn)->dx + KvadGetHexel(ptr, oldz, oldn)->dy * 3;
                
                KvadGetHexel(ptr, newz, newn)->val2 = blowpower;
            }
        }
    }
    
    
    // printf("\nez %i\ten %i", p_e->z, p_e->n);
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

void Repulsion(Kvad_t* ptr, int z, int n, int* dz, int* dn, int empty_st8)
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
}

void RelToAbs(int fx, int fy, int rel, int* dz, int* dn)
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

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0;

    int right_sum = 0, left_sum = 0;

    RelToAbs(fx, fy, 2, &rbx, &rby);
    RelToAbs(fx, fy, -2, &lbx, &lby);

    RelToAbs(fx, fy, 1, &rfx, &rfy);
    RelToAbs(fx, fy, -1, &lfx, &lfy);

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

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0;

    int right_sum = 0, left_sum = 0;

    RelToAbs(fx, fy, 2, &rbx, &rby);
    RelToAbs(fx, fy, -2, &lbx, &lby);

    RelToAbs(fx, fy, 1, &rfx, &rfy);
    RelToAbs(fx, fy, -1, &lfx, &lfy);

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

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_back = 0, b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0;
    
    int density = KvadGetHexel(ptr, z, n)->dns;

    RelToAbs(fx, fy, 2, &rbx, &rby);
    RelToAbs(fx, fy, -2, &lbx, &lby);

    RelToAbs(fx, fy, 1, &rfx, &rfy);
    RelToAbs(fx, fy, -1, &lfx, &lfy);
    
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

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_forward = 0, b_back = 0,
    b_rightfront = 0, b_leftfront = 0,
    b_rightback = 0, b_leftback = 0;

    int right_sum = 0, left_sum = 0, back_sum = 0, front_sum = 0;

    RelToAbs(fx, fy, 2, &rbx, &rby);
    RelToAbs(fx, fy, -2, &lbx, &lby);

    RelToAbs(fx, fy, 1, &rfx, &rfy);
    RelToAbs(fx, fy, -1, &lfx, &lfy);

    b_back =
    (
        !(KvadGetHexel(ptr, z + fx, n + fy)->dns < KvadGetHexel(ptr, z, n)->dns)
    );
    b_forward =
    (
        KvadGetHexel(ptr, z + fx, n + fy)->dns < KvadGetHexel(ptr, z, n)->dns
    );

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
    back_sum =
    (
        (KvadGetHexel(ptr, z - fx, n - fy)->dns >= KvadGetHexel(ptr, z, n)->dns) +
        (KvadGetHexel(ptr, z + lbx, n + lby)->dns >= KvadGetHexel(ptr, z, n)->dns) + 
        (KvadGetHexel(ptr, z + rbx, n + rby)->dns >= KvadGetHexel(ptr, z, n)->dns)
    );
    front_sum =
    (
        (KvadGetHexel(ptr, z + fx, n + fy)->dns >= KvadGetHexel(ptr, z, n)->dns) +
        (KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= KvadGetHexel(ptr, z, n)->dns) + 
        (KvadGetHexel(ptr, z + lfx, n + lfy)->dns >= KvadGetHexel(ptr, z, n)->dns)
    );
    
    if(right_sum > left_sum)
    {
        if(back_sum >= front_sum) b_leftfront = 1;
        else b_leftback = 1;
    } 
    if(right_sum < left_sum)
    {
        if(back_sum >= front_sum) b_rightfront = 1;
        else b_rightback = 1;
    }

    if(b_leftback || b_rightback) b_back = 0;
    if(b_leftfront || b_rightfront) b_forward = 0;
    
    
    // if(b_back       )  *dz -= fx,  *dn -= fy;
    if(b_forward    )  *dz += fx,  *dn += fy;
    if(b_rightfront )  *dz += rfx, *dn += rfy;
    if(b_leftfront  )  *dz += lfx, *dn += lfy;
    if(b_rightback  )  *dz += rbx, *dn += rby;
    if(b_leftback   )  *dz += lbx, *dn += lby;
    
}

void ForceIce(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{
    *dz = 0, *dn = 0;

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_forward = 0, b_back = 0,
    b_rightfront = 0, b_leftfront = 0,
    b_rightback = 0, b_leftback = 0;

    RelToAbs(fx, fy, 2, &rbx, &rby);
    RelToAbs(fx, fy, -2, &lbx, &lby);

    RelToAbs(fx, fy, 1, &rfx, &rfy);
    RelToAbs(fx, fy, -1, &lfx, &lfy);

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
    
    // if(b_back       )  *dz -= fx,  *dn -= fy;
    if(b_forward    )  *dz += fx,  *dn += fy;
    if(b_rightfront )  *dz += rbx, *dn += rby;
    if(b_leftfront  )  *dz += lbx, *dn += lby;
    if(b_rightback  )  *dz += rbx, *dn += rby;
    if(b_leftback   )  *dz += lbx, *dn += lby;
}

void ForceGas(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{
    *dz = 0, *dn = 0;

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_back = 0, b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0, 
    b_rightback = 0, b_leftback = 0;

    int right_sum = 0, left_sum = 0, back_sum = 0, front_sum = 0;

    RelToAbs(fx, fy, 2, &rbx, &rby);
    RelToAbs(fx, fy, -2, &lbx, &lby);

    RelToAbs(fx, fy, 1, &rfx, &rfy);
    RelToAbs(fx, fy, -1, &lfx, &lfy);


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

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_back = 0, b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0;

    int right_sum = 0, left_sum = 0;
    int state = KvadGetHexel(ptr, z, n)->st8;

    RelToAbs(fx, fy, 2, &rbx, &rby);
    RelToAbs(fx, fy, -2, &lbx, &lby);

    RelToAbs(fx, fy, 1, &rfx, &rfy);
    RelToAbs(fx, fy, -1, &lfx, &lfy);

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
    
    // if(b_back        )  *dz -= fx,  *dn -= fy;
    if(b_forward     )  *dz += fx,  *dn += fy;
    if(b_rightfront  )  *dz += rfx, *dn += rfy;
    if(b_leftfront   )  *dz += lfx, *dn += lfy;
}

void ForceRigid(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{   
    *dz = 0, *dn = 0;

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_forward = 0, 
    b_rightfront = 0, b_leftfront = 0;
    
    int density = KvadGetHexel(ptr, z, n)->dns;

    RelToAbs(fx, fy, 2, &rbx, &rby);
    RelToAbs(fx, fy, -2, &lbx, &lby);

    RelToAbs(fx, fy, 1, &rfx, &rfy);
    RelToAbs(fx, fy, -1, &lfx, &lfy);

    b_forward =
    (
        (
            KvadGetHexel(ptr, z + fx, n + fy)->dns     < density &&
            KvadGetHexel(ptr, z + rfx, n + rfy)->dns   < density &&
            KvadGetHexel(ptr, z + lfx, n + lfy)->dns   < density
        )
        
    );
    if(b_forward     )  *dz += fx,  *dn += fy;
    if(b_rightfront  )  *dz += rfx, *dn += rfy;
    if(b_leftfront   )  *dz += lfx, *dn += lfy;
}

void ForceViscous(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn)
{
    *dz = 0, *dn = 0;

    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    
    int b_forward = 0, b_back = 0,
    b_rightfront = 0, b_leftfront = 0,
    b_rightback = 0, b_leftback = 0;

    RelToAbs(fx, fy, 2, &rbx, &rby);
    RelToAbs(fx, fy, -2, &lbx, &lby);

    RelToAbs(fx, fy, 1, &rfx, &rfy);
    RelToAbs(fx, fy, -1, &lfx, &lfy);

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
            KvadGetHexel(ptr, z + lbx, n + lby)->dns < KvadGetHexel(ptr, z, n)->dns && 0
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
            KvadGetHexel(ptr, z + lbx, n + lby)->dns < KvadGetHexel(ptr, z, n)->dns && 0
        )
    );
    b_rightback =
    (
        (
            KvadGetHexel(ptr, z + fx, n + fy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            // KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            // KvadGetHexel(ptr, z + lfx, n + lfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + rbx, n + rby)->dns < KvadGetHexel(ptr, z, n)->dns &&
            KvadGetHexel(ptr, z + lbx, n + lby)->dns >= KvadGetHexel(ptr, z, n)->dns
        )
    );
    b_leftback =
    (
        (
            KvadGetHexel(ptr, z + fx, n + fy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            // KvadGetHexel(ptr, z + rfx, n + rfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
            // KvadGetHexel(ptr, z + lfx, n + lfy)->dns >= KvadGetHexel(ptr, z, n)->dns &&
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

void ContourPressure(Kvad_t* ptr, int j, int i)
{
    int curj = j, curi = i, curmat = KvadGetHexel(ptr, j, i)->mat;
    int curdns = KvadGetHexel(ptr, j, i)->dns;
    int addstr = 0;
    
    int gx = gravx, gy = gravy;
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    int grfx, grfy, glfx, glfy, grbx, grby, glbx, glby;
    
    int minimum = 0;
    int counter = 0;
    int turn;
    int pressure;
    
    RelToAbs(gravx, gravy, 2, &grbx, &grby);
    RelToAbs(gravx, gravy, -2, &glbx, &glby);
    RelToAbs(gravx, gravy, 1, &grfx, &grfy);
    RelToAbs(gravx, gravy, -1, &glfx, &glfy);
    
    gx = gravx, gy = gravy;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    do
    {
        if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
        && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat)
        {
            curj += lfx, curi += lfy;
            addstr = 0;
            if(KvadGetHexel(ptr, curj - gravx, curi - gravy)->dns > curdns)
            {
                // addstr = abs(KvadGetHexel(ptr, curj - gravx, curi - gravy)->stress);
            }
            
            if(lfx == gravx && lfy == gravy)
                counter += 2;
            if(lfx == -gravx && lfy == -gravy)
                counter -= 2;
                
            if((lfx == grfx && lfy == grfy)
            || (lfx == glfx && lfy == glfy))
                counter += 1;
            if((lfx == grbx && lfy == grby)
            || (lfx == glbx && lfy == glby))
                counter -= 1;
                
            gx = rbx, gy = rby;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            pressure = counter - addstr + 0 * NeighbourCount(ptr, j, i, curmat);
            KvadGetHexel(ptr, curj, curi)->stress = pressure;
            minimum = hmin(minimum, pressure);
            
            turn = 6;
        }
        else
        {
            gx = lfx, gy = lfy;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            turn++;
        }
    }
    while(!(curj == j && curi == i) || turn < 6);
    
    gx = gravx, gy = gravy;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    do
    {
        if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
        && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat)
        {
            curj += lfx, curi += lfy;
                
            gx = rbx, gy = rby;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            KvadGetHexel(ptr, curj, curi)->stress -= minimum;
            
            turn = 6;
        }
        else
        {
            gx = lfx, gy = lfy;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            turn++;
        }
    }
    while(!(curj == j && curi == i) || turn < 6);

}

void ContourSetUp(Kvad_t* ptr, int j0, int i0)
{
    int curj = j0, curi = i0, curmat = KvadGetHexel(ptr, j0, i0)->mat;
    int curdns = KvadGetHexel(ptr, j0, i0)->dns;
    int addstr = 0;
    
    int gx = gravx, gy = gravy;
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    int grfx, grfy, glfx, glfy, grbx, grby, glbx, glby;
    
    int minimum = 0;
    int counter = 0;
    int turn;
    int pressure;
    
    int gx2 = 0, gy2 = 0;
    int dz = 0, dn = 0;
    
    int priority = 0;
    int b_dir_fall = 0;
    int b_up_fall = 0;
    int field_value = 0;
    
    int gdx = 0, gdy = 0;
    
    int rfx2, rfy2, lfx2, lfy2, rbx2, rby2, lbx2, lby2;
    
    RelToAbs(gravx, gravy, 2, &grbx, &grby);
    RelToAbs(gravx, gravy, -2, &glbx, &glby);
    RelToAbs(gravx, gravy, 1, &grfx, &grfy);
    RelToAbs(gravx, gravy, -1, &glfx, &glfy);
    
    gx2 = -gravx, gy2 = -gravy;
    
    RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
    RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
    RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
    RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
    
    turn = 0;
    if(NeighbourCount(ptr, curj, curi, curmat) < 6)
    {
        do
        {
            if(NeighbourCount(ptr, curj + lfx2, curi + lfy2, curmat) < 6
            && KvadGetHexel(ptr, curj + lfx2, curi + lfy2)->mat == curmat)
            {
                curj += lfx2, curi += lfy2;
                addstr = 0;
                    
                gx2 = rbx2, gy2 = rby2;
                
                RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
                RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
                RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
                RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
                
                int j = curj, i = curi;
                
                KvadGetHexel(ptr, j, i)->tmp = KvadGetHexel(ptr, j, i)->mat;
                
                turn = 6;
            }
            else
            {
                gx2 = lfx2, gy2 = lfy2;
                
                RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
                RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
                RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
                RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
                
                turn++;
            }
        }
        while(!(curj == j0 && curi == i0) || turn < 6);
        
        gx2 = -gravx, gy2 = -gravy;
        
        RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
        RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
        RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
        RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
        
        turn = 0;
        
        do
        {
            if(NeighbourCount(ptr, curj + lfx2, curi + lfy2, curmat) < 6
            && KvadGetHexel(ptr, curj + lfx2, curi + lfy2)->mat == curmat)
            {
                curj += lfx2, curi += lfy2;
                addstr = 0;
                    
                gx2 = rbx2, gy2 = rby2;
                
                RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
                RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
                RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
                RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
                
                int j = curj, i = curi;
                
                KvadGetHexel(ptr, j, i)->fld2 = 0;
                switch (KvadGetHexel(ptr, j, i)->st8)
                {
                case 9:
                    if(KvadGetHexel(ptr, j, i)->ded)
                    {
                        KvadGetHexel(ptr, j, i)->fld = 0;
                        KvadGetHexel(ptr, j, i)->stress = 0;
                    }
                    else
                    {
                    KvadGetHexel(ptr, j, i)->fld = -10;
                    KvadGetHexel(ptr, j, i)->dx = 0;
                    KvadGetHexel(ptr, j, i)->dy = 0;
                    }
                    break;
                
                
                default:
                    KvadGetHexel(ptr, j, i)->fld = 0;
                    KvadGetHexel(ptr, j, i)->stress = 0;
                    break;
                }
                
                turn = 6;
            }
            else
            {
                gx2 = lfx2, gy2 = lfy2;
                
                RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
                RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
                RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
                RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
                
                turn++;
            }
        }
        while(!(curj == j0 && curi == i0) || turn < 6);
        
        gx2 = -gravx, gy2 = -gravy;
        
        RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
        RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
        RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
        RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
        
        turn = 0;
        
        do
        {
            if(NeighbourCount(ptr, curj + lfx2, curi + lfy2, curmat) < 6
            && KvadGetHexel(ptr, curj + lfx2, curi + lfy2)->mat == curmat)
            {
                curj += lfx2, curi += lfy2;
                addstr = 0;
                    
                gx2 = rbx2, gy2 = rby2;
                
                RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
                RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
                RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
                RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
                
                {
                int j = curj, i = curi;
                
                gx = KvadGetHexel(ptr, j, i)->dx;
                gy = KvadGetHexel(ptr, j, i)->dy;
                
                if(t == gravtime
                && hdist(0, 0, gx + gravx, gy + gravy) < 2)
                {
                    switch (KvadGetHexel(ptr, j, i)->st8)
                    {
                    case 1:
                        gx -= gravx;
                        gy -= gravy;
                        break;
                    case 9:
                        if(KvadGetHexel(ptr, j, i)->ded)
                        {
                            gx += gravx;
                            gy += gravy;
                        }
                        break;
                    
                    default:
                        gx += gravx;
                        gy += gravy;
                        break;
                    }
                    
                    
                }
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);

                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                priority = 0;
                switch (KvadGetHexel(ptr, j, i)->st8)
                {
                case 1:
                    
                    ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                    
                    break;
                case 2:
                    
                    ForceRock(ptr, j, i, gx, gy, &dz, &dn);
                    
                    break;
                case 3:
                
                    ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                
                    break;
                case 4:
                    
                    ForceDirt(ptr, j, i, gx, gy, &dz, &dn);
                    if(dz == gx && dn == gy)
                    {
                        priority = 1;
                    }
                    
                    break;
                case 5:
                    
                    ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                    b_up_fall = dz == -gx && dn == -gy;
                    b_dir_fall = dz == gx && dn == gy;
                    
                    priority = b_dir_fall + b_up_fall * 2;
                    
                    break;
                case 6:
                    
                    ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                    if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                    {    
                        priority = 1;
                    }
                    if( (dz == gx && dn == gy) )
                    {    
                        priority = 2;
                    }
                    
                    break;
                case 7:
                    
                    ForceIce(ptr, j, i, gx, gy, &dz, &dn);
                    b_up_fall = dz == -gx && dn == -gy;
                    b_dir_fall = dz == gx && dn == gy;
                    
                    priority = b_dir_fall + b_up_fall * 2;
                    
                    break;
                case 8:
                
                    ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                    if(dz == gx && dn == gy)
                    {    
                        priority = 1;
                        
                    }
                    if(dz == -gx && dn == -gy)
                    {    
                        priority = 2;
                        
                    }
                    
                    break;
                case 9:
                
                    ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                    
                    break;
                case 10:
                
                    ForceViscous(ptr, j, i, gx, gy, &dz, &dn);
                    b_up_fall = dz == -gx && dn == -gy;
                    b_dir_fall = dz == gx && dn == gy;
                    
                    priority = b_dir_fall + b_up_fall * 2;
                    
                    break;
                default:
                    dz = gx, dn = gy;
                    break;
                }
                
                if(KvadGetHexel(ptr, j, i)->val2 > 5)
                {
                    ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                    if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                    {    
                        priority = 1;
                        
                    }
                    if( (dz == gx && dn == gy) )
                    {    
                        priority = 2;
                        
                    }
                }
                
                field_value = KvadGetHexel(ptr, j, i)->dns + priority;
                    
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
                
                turn = 6;
            }
            else
            {
                gx2 = lfx2, gy2 = lfy2;
                
                RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
                RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
                RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
                RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
                
                turn++;
            }
        }
        while(!(curj == j0 && curi == i0) || turn < 6);
        
    }
}

void ContourPhysics(Kvad_t* ptr, int j0, int i0)
{
    int curj = j0, curi = i0, curmat = KvadGetHexel(ptr, j0, i0)->mat;
    int curdns = KvadGetHexel(ptr, j0, i0)->dns;
    int addstr = 0;
    
    int gx = gravx, gy = gravy;
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    int grfx, grfy, glfx, glfy, grbx, grby, glbx, glby;
    
    int minimum = 0;
    int counter = 0;
    int turn;
    int pressure;
    
    int gx2 = 0, gy2 = 0;
    int dz = 0, dn = 0;
    
    int priority = 0;
    int b_dir_fall = 0;
    int b_up_fall = 0;
    int field_value = 0;
    
    int gdx = 0, gdy = 0;
    
    int rfx2, rfy2, lfx2, lfy2, rbx2, rby2, lbx2, lby2;
    
    RelToAbs(gravx, gravy, 2, &grbx, &grby);
    RelToAbs(gravx, gravy, -2, &glbx, &glby);
    RelToAbs(gravx, gravy, 1, &grfx, &grfy);
    RelToAbs(gravx, gravy, -1, &glfx, &glfy);
    
    gx2 = -gravx, gy2 = -gravy;
    
    RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
    RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
    RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
    RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
    
    turn = 0;
    if(NeighbourCount(ptr, curj, curi, curmat) < 6)
    {   
        do
        {
            if(NeighbourCount(ptr, curj + lfx2, curi + lfy2, curmat) < 6
            && KvadGetHexel(ptr, curj + lfx2, curi + lfy2)->mat == curmat)
            {
                curj += lfx2, curi += lfy2;
                addstr = 0;
                    
                gx2 = rbx2, gy2 = rby2;
                
                RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
                RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
                RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
                RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
                
                {
                int j = curj, i = curi;
                
                gx = KvadGetHexel(ptr, j, i)->dx;
                gy = KvadGetHexel(ptr, j, i)->dy;
                
                if(t == gravtime
                && hdist(0, 0, gx + gravx, gy + gravy) < 2)
                {
                    switch (KvadGetHexel(ptr, j, i)->st8)
                    {
                    case 1:
                        gx -= gravx;
                        gy -= gravy;
                        break;
                    case 9:
                        if(KvadGetHexel(ptr, j, i)->ded)
                        {
                            gx += gravx;
                            gy += gravy;
                        }
                        break;
                    
                    default:
                        gx += gravx;
                        gy += gravy;
                        break;
                    }
                }
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);

                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                priority = 0;
                switch (KvadGetHexel(ptr, j, i)->st8)
                {
                case 1:
                    
                    ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                    // Repulsion(ptr, j, i, &dz, &dn, 0);
                    
                    break;
                case 2:
                    
                    ForceRock(ptr, j, i, gx, gy, &dz, &dn);
                    
                    break;
                case 3:
                    
                    ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                    
                    break;
                case 4:
                
                    ForceDirt(ptr, j, i, gx, gy, &dz, &dn);
                    if(dz == gx && dn == gy)
                    {
                        priority = 1;
                    }
                    
                    
                    
                    break;
                case 5:
                    
                    ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                    if(dz == gx && dn == gy)
                    {    
                        priority = 1;
                        
                    }
                    if(dz == -gx && dn == -gy)
                    {    
                        priority = 2;
                        
                    }
                    
                    break;
                case 6:
            
                    ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                    if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                    {    
                        priority = 1;
                        
                    }
                    if( (dz == gx && dn == gy) )
                    {    
                        priority = 2;
                        
                    }
                    
                    break;
                case 7:
                    
                    ForceIce(ptr, j, i, gx, gy, &dz, &dn);
                    if(dz == gx && dn == gy)
                    {    
                        priority = 1;
                        
                    }
                    if(dz == -gx && dn == -gy)
                    {    
                        priority = 2;
                        
                    }
                    
                    break;
                case 8:
                
                    ForceRope(ptr, j, i, gx, gy, &dz, &dn);
                    if(dz == gx && dn == gy)
                    {    
                        priority = 1;
                        
                    }
                    if(dz == -gx && dn == -gy)
                    {    
                        priority = 2;
                        
                    }
                    
                    break;
                case 9:
                
                    ForceSand(ptr, j, i, gx, gy, &dz, &dn);
                    
                    break;
                case 10:
                
                    ForceViscous(ptr, j, i, gx, gy, &dz, &dn);
                    if(dz == gx && dn == gy)
                    {    
                        priority = 1;
                        
                    }
                    if(dz == -gx && dn == -gy)
                    {    
                        priority = 2;
                        
                    }
                    
                    break;
                default:
                    dz = gx, dn = gy;
                    break;
                }
                
                if(KvadGetHexel(ptr, j, i)->val2 > 5)
                {
                    ForceLiquid(ptr, j, i, gx, gy, &dz, &dn);
                    if( (dz == rbx && dn == rby) || (dz == lbx && dn == lby) )
                    {    
                        priority = 1;
                        
                    }
                    if( (dz == gx && dn == gy) )
                    {    
                        priority = 2;
                        
                    }
                }
                
                field_value = KvadGetHexel(ptr, j, i)->dns + priority;
                if(KvadGetHexel(ptr, j + dz, i + dn)->fld == field_value &&
                    KvadGetHexel(ptr, j, i)->st8 != 0)
                {
                    swap(&KvadGetHexel(ptr, j     , i     )->tmp,
                        &KvadGetHexel(ptr, j + dz, i + dn)->tmp);
                    swap(&KvadGetHexel(ptr, j     , i     )->nrj,
                        &KvadGetHexel(ptr, j + dz, i + dn)->nrj);
                }
                }
                
                turn = 6;
            }
            else
            {
                gx2 = lfx2, gy2 = lfy2;
                
                RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
                RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
                RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
                RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
                
                turn++;
            }
        }
        while(!(curj == j0 && curi == i0) || turn < 6);
    }
}

void ContourUpdate(Kvad_t* ptr, int j0, int i0)
{
    int curj = j0, curi = i0, curmat = KvadGetHexel(ptr, j0, i0)->mat;
    int curdns = KvadGetHexel(ptr, j0, i0)->dns;
    int addstr = 0;
    
    int gx = gravx, gy = gravy;
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    int grfx, grfy, glfx, glfy, grbx, grby, glbx, glby;
    
    int minimum = 0;
    int counter = 0;
    int turn;
    int pressure;
    
    int gx2 = 0, gy2 = 0;
    int dz = 0, dn = 0;
    
    int priority = 0;
    int b_dir_fall = 0;
    int b_up_fall = 0;
    int field_value = 0;
    
    int gdx = 0, gdy = 0;
    
    int rfx2, rfy2, lfx2, lfy2, rbx2, rby2, lbx2, lby2;
    
    RelToAbs(gravx, gravy, 2, &grbx, &grby);
    RelToAbs(gravx, gravy, -2, &glbx, &glby);
    RelToAbs(gravx, gravy, 1, &grfx, &grfy);
    RelToAbs(gravx, gravy, -1, &glfx, &glfy);
    
    gx2 = -gravx, gy2 = -gravy;
    
    RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
    RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
    RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
    RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
    
    turn = 0;
    if(NeighbourCount(ptr, curj, curi, curmat) < 6)
    {
        do
        {
            if(NeighbourCount(ptr, curj + lfx2, curi + lfy2, curmat) < 6
            && KvadGetHexel(ptr, curj + lfx2, curi + lfy2)->mat == curmat)
            {
                curj += lfx2, curi += lfy2;
                addstr = 0;
                    
                gx2 = rbx2, gy2 = rby2;
                
                RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
                RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
                RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
                RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
                
                int j = curj, i = curi;
                
                KvadSetMat(ptr, j, i, KvadGetHexel(ptr, j, i)->tmp, 0);
                
                KvadGetHexel(ptr, j, i)->val2 = KvadGetHexel(ptr, j, i)->v2t;
                KvadGetHexel(ptr, j, i)->v2t = 0;
                
                turn = 6;
            }
            else
            {
                gx2 = lfx2, gy2 = lfy2;
                
                RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
                RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
                RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
                RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
                
                turn++;
            }
        }
        while(!(curj == j0 && curi == i0) || turn < 6);
    }
}

void ContourDebug(Kvad_t* ptr, int j0, int i0)
{
    int curj = j0, curi = i0, curmat = KvadGetHexel(ptr, j0, i0)->mat;
    int curdns = KvadGetHexel(ptr, j0, i0)->dns;
    int addstr = 0;
    
    int gx = gravx, gy = gravy;
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    int grfx, grfy, glfx, glfy, grbx, grby, glbx, glby;
    
    int minimum = 0;
    int counter = 0;
    int turn;
    int pressure;
    
    int gx2 = 0, gy2 = 0;
    int dz = 0, dn = 0;
    
    int priority = 0;
    int b_dir_fall = 0;
    int b_up_fall = 0;
    int field_value = 0;
    
    int gdx = 0, gdy = 0;
    
    int rfx2, rfy2, lfx2, lfy2, rbx2, rby2, lbx2, lby2;
    
    RelToAbs(gravx, gravy, 2, &grbx, &grby);
    RelToAbs(gravx, gravy, -2, &glbx, &glby);
    RelToAbs(gravx, gravy, 1, &grfx, &grfy);
    RelToAbs(gravx, gravy, -1, &glfx, &glfy);
    
    gx2 = -gravx, gy2 = -gravy;
    
    RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
    RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
    RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
    RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
    
    turn = 0;
    if(NeighbourCount(ptr, curj, curi, curmat) < 6)
    {
        do
        {
            if(NeighbourCount(ptr, curj + lfx2, curi + lfy2, curmat) < 6
            && KvadGetHexel(ptr, curj + lfx2, curi + lfy2)->mat == curmat)
            {
                curj += lfx2, curi += lfy2;
                addstr = 0;
                    
                gx2 = rbx2, gy2 = rby2;
                
                RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
                RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
                RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
                RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
                
                int j = curj, i = curi;
                
                KvadGetHexel(ptr, j, i)->clr = 5;
                printf("%i %i\n", j, i);
                
                turn = 6;
            }
            else
            {
                gx2 = lfx2, gy2 = lfy2;
                
                RelToAbs(gx2, gy2, 2, &rbx2, &rby2);
                RelToAbs(gx2, gy2, -2, &lbx2, &lby2);
                RelToAbs(gx2, gy2, 1, &rfx2, &rfy2);
                RelToAbs(gx2, gy2, -1, &lfx2, &lfy2);
                
                printf("%i %i\n", curj, curi);
                
                turn++;
            }
        }
        while(!(curj == j0 && curi == i0) || turn < 6);
    }
}

void ContourMove(Kvad_t* ptr, int j, int i)
{
    int curj = j, curi = i, curmat = KvadGetHexel(ptr, j, i)->mat;
    
    int gx = gravx, gy = gravy;
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    int grfx, grfy, glfx, glfy, grbx, grby, glbx, glby;
    
    int minimum = 0;
    int maximum = 0;
    int counter = 0;
    int deleted = 0;
    int contourcounter = 0;
    int leftcounter = 0;
    int rightcounter = 0;
    int turn;
    int maxadd = 100;
    
    int extendbottom = 0;
    int bottomcounter = 0;
    int extendleftside = 0;
    int extendrightside = 0;
    int extendtop = 0;
    int inverted = 0;
    int invextendtop = 0;
    int invextendleftside = 0;
    int invextendrightside = 0;
    
    RelToAbs(gravx, gravy, 2, &grbx, &grby);
    RelToAbs(gravx, gravy, -2, &glbx, &glby);
    RelToAbs(gravx, gravy, 1, &grfx, &grfy);
    RelToAbs(gravx, gravy, -1, &glfx, &glfy);
    
    
    gx = grbx, gy = grby;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    do
    {
        if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
        && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
        || NeighbourCount(ptr, curj, curi, curmat) == 0)
        {
            if(KvadGetHexel(ptr, curj, curi)->stress >= 0)
            {
                
                
                minimum = hmin(minimum, KvadGetHexel(ptr, curj, curi)->stress);
                if(NeighbourCount(ptr, curj, curi, 0) > 0)
                {
                    maximum = hmax(maximum, KvadGetHexel(ptr, curj, curi)->stress);
                    
                }
            
                contourcounter++;
                KvadGetHexel(ptr, curj, curi)->stress = -1 - abs(KvadGetHexel(ptr, curj, curi)->stress);
            }
            
            
            if(KvadGetHexel(ptr, curj + gravx, curi + gravy)->tmp == 0)
            {
                extendbottom = 1;
                bottomcounter++;
            }
        
            if(NeighbourCount(ptr, curj, curi, curmat) != 0)
            {
                if(KvadGetHexel(ptr, curj + lbx, curi + lby)->mat != curmat
                && NeighbourCount(ptr, curj, curi, curmat) > 4)
                    inverted = 1;
                
                curj += lfx, curi += lfy;
                
            }
            
            gx = rbx, gy = rby;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            
            turn = 6;
            
            
        }
        else
        {
            gx = lfx, gy = lfy;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            turn++;
        }
    }
    while(!(curj == j && curi == i) || turn < 6);
    
    printf("inverted %i\n", inverted);
    
    printf("contour %i\n", contourcounter);
    maxadd = contourcounter;
    
    if(extendbottom == 0 || bottomcounter * 4 < contourcounter || inverted)
    {
        gx = grbx, gy = grby;
        
        RelToAbs(gx, gy, 2, &rbx, &rby);
        RelToAbs(gx, gy, -2, &lbx, &lby);
        RelToAbs(gx, gy, 1, &rfx, &rfy);
        RelToAbs(gx, gy, -1, &lfx, &lfy);
        
        turn = 0;
        
        do
        {
            
            
            if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
            && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
            || NeighbourCount(ptr, curj, curi, curmat) == 0)
            {
                if(KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp == 0 
                    && !(KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp == 0))
                {
                    extendleftside = 1;
                }
                if(KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp == 0
                    && !(KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp == 0))
                {
                    extendrightside = 1;
                }
                if(inverted 
                && KvadGetHexel(ptr, curj - gravx, curi - gravy)->tmp == 0)
                {
                    invextendtop = 1;
                }
                if(inverted 
                && KvadGetHexel(ptr, curj + grbx, curi + grby)->tmp == 0)
                {
                    invextendrightside = 1;
                }
                if(inverted 
                && KvadGetHexel(ptr, curj + glbx, curi - glby)->tmp == 0)
                {
                    invextendleftside = 1;
                }
                
            
                if(NeighbourCount(ptr, curj, curi, curmat) != 0)
                    curj += lfx, curi += lfy;
                
                gx = rbx, gy = rby;
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);
                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                
                
                turn = 6;
            }
            else
            {
                gx = lfx, gy = lfy;
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);
                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                
                turn++;
            }
        }
        while(!(curj == j && curi == i) || turn < 6);
    }
    printf("invextendtop %i\n", invextendtop);
    if(extendbottom == 0 && extendleftside == 0 && extendrightside == 0) extendtop = 1;
    
    // if(leftcounter > rightcounter && leftcounter > 0) extendleftside = 1, extendrightside = 0;
    // if(rightcounter > leftcounter && rightcounter > 0) extendleftside = 0, extendrightside = 1;
    // if(rightcounter == leftcounter) extendleftside = 0, extendrightside = 0;
    
    gx = grbx, gy = grby;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    gx = grbx, gy = grby;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    
    do
    {
        if(KvadGetHexel(ptr, curj, curi)->stress < 0)
        {
            KvadGetHexel(ptr, curj, curi)->stress = -1 + abs(KvadGetHexel(ptr, curj, curi)->stress);
        }
        if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
        && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
        || NeighbourCount(ptr, curj, curi, curmat) == 0)
        {
            
            if(extendbottom == 1 && counter < maxadd)
            {
                if(KvadGetHexel(ptr, curj + gravx, curi + gravy)->tmp == 0)
                {
                    KvadGetHexel(ptr, curj + gravx, curi + gravy)->tmp = curmat;
                        counter++;
                }
            }
            
            {
                if(extendleftside == 1 && counter < maxadd)
                {
                    if(KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp == 0 
                    && !(KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp == 0)
                    )
                    {
                        KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp = curmat;
                        // KvadGetHexel(ptr, curj + glfx, curi + glfy)->clr = 6;
                        // KvadGetHexel(ptr, curj, curi)->clr = 1;
                        counter++;
                    }
                }
                if(extendrightside == 1 && counter < maxadd)
                {
                    if(KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp == 0
                    && !(KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp == 0)
                    )
                    {
                        KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp = curmat;
                        // KvadGetHexel(ptr, curj + grfx, curi + grfy)->clr = 6;
                        // KvadGetHexel(ptr, curj, curi)->clr = 2;
                        counter++;
                    }
                }
                if(extendtop == 1 && counter < maxadd
                && KvadGetHexel(ptr, curj, curi)->stress > 2)
                {
                    if(KvadGetHexel(ptr, curj - gravx, curi - gravy)->tmp == 0
                    && KvadGetHexel(ptr, curj, curi)->stress == maximum)
                    {
                        KvadGetHexel(ptr, curj - gravx, curi - gravy)->tmp = curmat;
                        counter++;
                    }
                }
            }
            
            
            
            
        
            if(NeighbourCount(ptr, curj, curi, curmat) != 0)
                curj += lfx, curi += lfy;
                
            gx = rbx, gy = rby;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            
            
            turn = 6;
        }
        else
        {
            gx = lfx, gy = lfy;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            turn++;
        }
        
    }
    while(!(curj == j && curi == i) || turn < 6);
    int nextmin = 0;
    printf("waterloop\t%i\n", counter);
    while(counter > 0)
    {
        curj = j, curi = i;
        
        gx = grbx, gy = grby;
        
        RelToAbs(gx, gy, 2, &rbx, &rby);
        RelToAbs(gx, gy, -2, &lbx, &lby);
        RelToAbs(gx, gy, 1, &rfx, &rfy);
        RelToAbs(gx, gy, -1, &lfx, &lfy);
        
        turn = 0;
        
        do
        {
            
            if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
            && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
            || NeighbourCount(ptr, curj, curi, curmat) == 0)
            {
                if(KvadGetHexel(ptr, curj, curi)->stress == (minimum + nextmin) && counter > 0
                && KvadGetHexel(ptr, curj, curi)->tmp != 0)
                {
                    KvadGetHexel(ptr, curj, curi)->tmp = 0;
                    counter--;
                    deleted++;
                }
            
                if(NeighbourCount(ptr, curj, curi, curmat) != 0)
                    curj += lfx, curi += lfy;
                    
                gx = rbx, gy = rby;
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);
                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                
                
                
                turn = 6;
            }
            else
            {
                gx = lfx, gy = lfy;
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);
                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                
                turn++;
            }
        }
        while(!(curj == j && curi == i) || turn < 6);
        
        if(minimum + nextmin > maximum + 1 && counter > 0)
        {
            counter--;
            printf("\033[0;30;41mERROR\n\033[0m");
        }
        
        nextmin++;
        
        
    }
    printf("deleted--\t%i\n", deleted);
    
    gx = grbx, gy = grby;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    do
    {
        
        if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
        && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
        || NeighbourCount(ptr, curj, curi, curmat) == 0)
        {
            if(KvadGetHexel(ptr, curj, curi)->stress >= 0)
            {
                KvadGetHexel(ptr, curj, curi)->stress = - 1 - abs(KvadGetHexel(ptr, curj, curi)->stress);
            }
        
            if(NeighbourCount(ptr, curj, curi, curmat) != 0)
                curj += lfx, curi += lfy;
                
            gx = rbx, gy = rby;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            
            
            turn = 6;
        }
        else
        {
            gx = lfx, gy = lfy;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            turn++;
        }
    }
    while(!(curj == j && curi == i) || turn < 6);

}

void ContourMoveSand(Kvad_t* ptr, int j, int i)
{
    int curj = j, curi = i, curmat = KvadGetHexel(ptr, j, i)->mat;
    
    int gx = gravx, gy = gravy;
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    int grfx, grfy, glfx, glfy, grbx, grby, glbx, glby;
    
    int minimum = 0;
    int maximum = 0;
    int counter = 0;
    int deleted = 0;
    int contourcounter = 0;
    int leftcounter = 0;
    int rightcounter = 0;
    int turn;
    int maxadd = 100;
    
    int extendbottom = 0;
    int bottomcounter = 0;
    int extendleftside = 0;
    int extendrightside = 0;
    int extendtop = 0;
    int inverted = 0;
    int invextendtop = 0;
    
    RelToAbs(gravx, gravy, 2, &grbx, &grby);
    RelToAbs(gravx, gravy, -2, &glbx, &glby);
    RelToAbs(gravx, gravy, 1, &grfx, &grfy);
    RelToAbs(gravx, gravy, -1, &glfx, &glfy);
    
    
    gx = grbx, gy = grby;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    do
    {
        if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
        && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
        || NeighbourCount(ptr, curj, curi, curmat) == 0)
        {
            if(KvadGetHexel(ptr, curj, curi)->stress >= 0)
            {
                
                
                minimum = hmin(minimum, KvadGetHexel(ptr, curj, curi)->stress);
                if(KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp != curmat
                || KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp != curmat)
                {
                    minimum = hmin(minimum, KvadGetHexel(ptr, curj, curi)->stress / 2);
                }
                if(NeighbourCount(ptr, curj, curi, 0) > 0)
                {
                    maximum = hmax(maximum, KvadGetHexel(ptr, curj, curi)->stress);
                    
                }
            
                contourcounter++;
                KvadGetHexel(ptr, curj, curi)->stress = -1 - abs(KvadGetHexel(ptr, curj, curi)->stress);
            }
            
            
            if(KvadGetHexel(ptr, curj + gravx, curi + gravy)->tmp == 0)
            {
                extendbottom = 1;
                bottomcounter++;
            }
        
            if(NeighbourCount(ptr, curj, curi, curmat) != 0)
            {
                if(KvadGetHexel(ptr, curj + lbx, curi + lby)->mat != curmat
                && NeighbourCount(ptr, curj, curi, curmat) > 4)
                    inverted = 1;
                
                curj += lfx, curi += lfy;
                
            }
            
            gx = rbx, gy = rby;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            
            turn = 6;
            
            
        }
        else
        {
            gx = lfx, gy = lfy;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            turn++;
        }
    }
    while(!(curj == j && curi == i) || turn < 6);
    
    if(doprint) printf("minimum %i\n", minimum);
    
    if(doprint) printf("contour %i\n", contourcounter);
    maxadd = contourcounter;
    
    if(extendbottom == 0 || bottomcounter * 4 < contourcounter || inverted)
    {
        gx = grbx, gy = grby;
        
        RelToAbs(gx, gy, 2, &rbx, &rby);
        RelToAbs(gx, gy, -2, &lbx, &lby);
        RelToAbs(gx, gy, 1, &rfx, &rfy);
        RelToAbs(gx, gy, -1, &lfx, &lfy);
        
        turn = 0;
        
        do
        {
            
            
            if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
            && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
            || NeighbourCount(ptr, curj, curi, curmat) == 0)
            {
                if(KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp == 0 
                    && !(KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp == 0))
                {
                    extendleftside = 1;
                }
                if(KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp == 0
                    && !(KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp == 0))
                {
                    extendrightside = 1;
                }
                if(inverted 
                && KvadGetHexel(ptr, curj - gravx, curi - gravy)->tmp == 0)
                {
                    invextendtop = 1;
                }
            
                if(NeighbourCount(ptr, curj, curi, curmat) != 0)
                    curj += lfx, curi += lfy;
                
                gx = rbx, gy = rby;
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);
                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                
                
                turn = 6;
            }
            else
            {
                gx = lfx, gy = lfy;
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);
                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                
                turn++;
            }
        }
        while(!(curj == j && curi == i) || turn < 6);
    }
    if(doprint) printf("invextendtop %i\n", invextendtop);
    if(extendbottom == 0 && extendleftside == 0 && extendrightside == 0) extendtop = 1;
    
    // if(leftcounter > rightcounter && leftcounter > 0) extendleftside = 1, extendrightside = 0;
    // if(rightcounter > leftcounter && rightcounter > 0) extendleftside = 0, extendrightside = 1;
    // if(rightcounter == leftcounter) extendleftside = 0, extendrightside = 0;
    
    gx = grbx, gy = grby;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    gx = grbx, gy = grby;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    if(inverted)
    {
        extendbottom = 0;
        extendleftside = 0;
        extendrightside = 0;
        
    }
    // extendbottom = 1;
    extendleftside = 0;
    extendrightside = 0;
    extendtop = 0;
    do
    {
        if(KvadGetHexel(ptr, curj, curi)->stress < 0)
        {
            KvadGetHexel(ptr, curj, curi)->stress = -1 + abs(KvadGetHexel(ptr, curj, curi)->stress);
        }
        if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
        && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
        || NeighbourCount(ptr, curj, curi, curmat) == 0)
        {
            
            if(inverted)
            {
            
            }
            
            if(extendbottom == 1 && counter < maxadd)
            {
                if(KvadGetHexel(ptr, curj + gravx, curi + gravy)->tmp == 0)
                {
                    KvadGetHexel(ptr, curj + gravx, curi + gravy)->tmp = curmat;
                        counter++;
                }
            }
            
            {
                if(extendleftside == 1 && counter < maxadd)
                {
                    if(KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp == 0 
                    && !(KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp == 0)
                    // && NeighbourCount(ptr, j, i, curmat) > 2
                    )
                    {
                        KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp = curmat;
                        // KvadGetHexel(ptr, curj + glfx, curi + glfy)->clr = 6;
                        // KvadGetHexel(ptr, curj, curi)->clr = 1;
                        counter++;
                    }
                }
                if(extendrightside == 1 && counter < maxadd)
                {
                    if(KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp == 0
                    && !(KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp == 0)
                    // && NeighbourCount(ptr, j, i, curmat) > 2
                    )
                    {
                        KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp = curmat;
                        // KvadGetHexel(ptr, curj + grfx, curi + grfy)->clr = 6;
                        // KvadGetHexel(ptr, curj, curi)->clr = 2;
                        counter++;
                    }
                }
                if(invextendtop && counter < maxadd
                && KvadGetHexel(ptr, curj - gravx, curi - gravy)->tmp == 0)
                {
                    KvadGetHexel(ptr, curj - gravx, curi - gravy)->tmp = curmat;
                        counter++;
                }
                if(extendtop == 1 && counter < maxadd)
                {
                    if(KvadGetHexel(ptr, curj - gravx, curi - gravy)->tmp == 0
                    && KvadGetHexel(ptr, curj, curi)->stress == maximum)
                    {
                        KvadGetHexel(ptr, curj - gravx, curi - gravy)->tmp = curmat;
                        counter++;
                    }
                }
            }
            
            
            
            
        
            if(NeighbourCount(ptr, curj, curi, curmat) != 0)
                curj += lfx, curi += lfy;
                
            gx = rbx, gy = rby;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            
            
            turn = 6;
        }
        else
        {
            gx = lfx, gy = lfy;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            turn++;
        }
        
    }
    while(!(curj == j && curi == i) || turn < 6);
    int nextmin = 0;
    if(doprint) printf("sandloop\t%i\n", counter);
    while(counter > 0)
    {
        curj = j, curi = i;
        
        gx = grbx, gy = grby;
        
        RelToAbs(gx, gy, 2, &rbx, &rby);
        RelToAbs(gx, gy, -2, &lbx, &lby);
        RelToAbs(gx, gy, 1, &rfx, &rfy);
        RelToAbs(gx, gy, -1, &lfx, &lfy);
        
        turn = 0;
        
        do
        {
            
            if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
            && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
            || NeighbourCount(ptr, curj, curi, curmat) == 0)
            {
                if((KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp != curmat
                || KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp != curmat)
                && KvadGetHexel(ptr, curj, curi)->stress / 2 == (minimum + nextmin) && counter > 0
                && KvadGetHexel(ptr, curj, curi)->tmp != 0)
                {
                    KvadGetHexel(ptr, curj, curi)->tmp = 0;
                    counter--;
                    deleted++;
                }
            
                // if(KvadGetHexel(ptr, curj, curi)->stress == (minimum + nextmin) && counter > 0
                // && KvadGetHexel(ptr, curj, curi)->tmp != 0)
                // {
                //     KvadGetHexel(ptr, curj, curi)->tmp = 0;
                //     counter--;
                //     deleted++;
                // }
            
                if(NeighbourCount(ptr, curj, curi, curmat) != 0)
                    curj += lfx, curi += lfy;
                    
                gx = rbx, gy = rby;
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);
                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                
                
                
                turn = 6;
            }
            else
            {
                gx = lfx, gy = lfy;
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);
                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                
                turn++;
            }
        }
        while(!(curj == j && curi == i) || turn < 6);
        
        if(minimum + nextmin > maximum + 1)
        {
            counter--;
            if(doprint) printf("\033[0;30;41mERROR\n\033[0m");
        }
        
        nextmin++;
        
        
    }
    if(doprint) printf("deleted--\t%i\n", deleted);
    
    gx = grbx, gy = grby;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    do
    {
        
        if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
        && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
        || NeighbourCount(ptr, curj, curi, curmat) == 0)
        {
            if(KvadGetHexel(ptr, curj, curi)->stress >= 0)
            {
                KvadGetHexel(ptr, curj, curi)->stress = - 1 - abs(KvadGetHexel(ptr, curj, curi)->stress);
            }
        
            if(NeighbourCount(ptr, curj, curi, curmat) != 0)
                curj += lfx, curi += lfy;
                
            gx = rbx, gy = rby;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            
            
            turn = 6;
        }
        else
        {
            gx = lfx, gy = lfy;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            turn++;
        }
    }
    while(!(curj == j && curi == i) || turn < 6);

}

void ContourMoveSolid(Kvad_t* ptr, int j, int i)
{
    int curj = j, curi = i, curmat = KvadGetHexel(ptr, j, i)->mat;
    
    int gx = gravx, gy = gravy;
    int rfx, rfy, lfx, lfy, rbx, rby, lbx, lby;
    int grfx, grfy, glfx, glfy, grbx, grby, glbx, glby;
    
    int minimum = 0;
    int maximum = 0;
    int counter = 0;
    int deleted = 0;
    int contourcounter = 0;
    int leftcounter = 0;
    int rightcounter = 0;
    int turn;
    int maxadd = 100;
    
    int extendbottom = 0;
    int bottomcounter = 0;
    int extendleftside = 0;
    int extendrightside = 0;
    int extendtop = 0;
    
    RelToAbs(gravx, gravy, 2, &grbx, &grby);
    RelToAbs(gravx, gravy, -2, &glbx, &glby);
    RelToAbs(gravx, gravy, 1, &grfx, &grfy);
    RelToAbs(gravx, gravy, -1, &glfx, &glfy);
    
    
    gx = grbx, gy = grby;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    do
    {
        if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
        && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
        || NeighbourCount(ptr, curj, curi, curmat) == 0)
        {
            if(KvadGetHexel(ptr, curj, curi)->stress >= 0)
            {
                
                
                minimum = hmin(minimum, KvadGetHexel(ptr, curj, curi)->stress);
                if(NeighbourCount(ptr, curj, curi, 0) > 0)
                {
                    maximum = hmax(maximum, KvadGetHexel(ptr, curj, curi)->stress);
                    
                }
            
                contourcounter++;
                KvadGetHexel(ptr, curj, curi)->stress = -1 - abs(KvadGetHexel(ptr, curj, curi)->stress);
            }
            
            
            if(KvadGetHexel(ptr, curj + gravx, curi + gravy)->tmp == 0)
            {
                extendbottom = 1;
                bottomcounter++;
            }
        
            if(NeighbourCount(ptr, curj, curi, curmat) != 0)
                curj += lfx, curi += lfy;
            
            gx = rbx, gy = rby;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            
            turn = 6;
            
            
        }
        else
        {
            gx = lfx, gy = lfy;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            turn++;
        }
    }
    while(!(curj == j && curi == i) || turn < 6);
    
    if(doprint) printf("contour %i\n", contourcounter);
    maxadd = contourcounter;
    
    if(extendbottom == 0 || bottomcounter * 4 < contourcounter || 1)
    {
        gx = grbx, gy = grby;
        
        RelToAbs(gx, gy, 2, &rbx, &rby);
        RelToAbs(gx, gy, -2, &lbx, &lby);
        RelToAbs(gx, gy, 1, &rfx, &rfy);
        RelToAbs(gx, gy, -1, &lfx, &lfy);
        
        turn = 0;
        
        do
        {
            
            
            if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
            && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
            || NeighbourCount(ptr, curj, curi, curmat) == 0)
            {
                if(KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp == 0 
                    && !(KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp == 0))
                {
                    extendleftside = 1;
                    // leftcounter++;
                }
                if(KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp == 0
                    && !(KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp == 0))
                {
                    extendrightside = 1;
                    // rightcounter++;
                }
            
                if(NeighbourCount(ptr, curj, curi, curmat) != 0)
                    curj += lfx, curi += lfy;
                
                gx = rbx, gy = rby;
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);
                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                
                
                turn = 6;
            }
            else
            {
                gx = lfx, gy = lfy;
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);
                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                
                turn++;
            }
        }
        while(!(curj == j && curi == i) || turn < 6);
    }
    
    if(extendbottom == 0 && extendleftside == 0 && extendrightside == 0) extendtop = 1;
    
    // if(leftcounter > rightcounter && leftcounter > 0) extendleftside = 1, extendrightside = 0;
    // if(rightcounter > leftcounter && rightcounter > 0) extendleftside = 0, extendrightside = 1;
    // if(rightcounter == leftcounter) extendleftside = 0, extendrightside = 0;
    
    gx = grbx, gy = grby;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    gx = grbx, gy = grby;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    // extendbottom = 0;
    extendleftside = 0;
    extendrightside = 0;
    extendtop = 0;
    
    do
    {
        if(KvadGetHexel(ptr, curj, curi)->stress < 0)
        {
            KvadGetHexel(ptr, curj, curi)->stress = -1 + abs(KvadGetHexel(ptr, curj, curi)->stress);
        }
        if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
        && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
        || NeighbourCount(ptr, curj, curi, curmat) == 0)
        {
        
            if(extendbottom == 1 && counter < maxadd)
            {
                if(KvadGetHexel(ptr, curj + gravx, curi + gravy)->tmp == 0)
                {
                    KvadGetHexel(ptr, curj + gravx, curi + gravy)->tmp = curmat;
                        counter++;
                }
            }
            else
            {
                if(extendleftside == 1 && counter < maxadd)
                {
                    if(KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp == 0 
                    && !(KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp == 0)
                    // && NeighbourCount(ptr, j, i, curmat) > 2
                    )
                    {
                        KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp = curmat;
                        // KvadGetHexel(ptr, curj + glfx, curi + glfy)->clr = 6;
                        // KvadGetHexel(ptr, curj, curi)->clr = 1;
                        counter++;
                    }
                }
                if(extendrightside == 1 && counter < maxadd)
                {
                    if(KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp == 0
                    && !(KvadGetHexel(ptr, curj + glfx, curi + glfy)->tmp == 0)
                    // && NeighbourCount(ptr, j, i, curmat) > 2
                    )
                    {
                        KvadGetHexel(ptr, curj + grfx, curi + grfy)->tmp = curmat;
                        // KvadGetHexel(ptr, curj + grfx, curi + grfy)->clr = 6;
                        // KvadGetHexel(ptr, curj, curi)->clr = 2;
                        counter++;
                    }
                }
                if(extendtop == 1 && counter < maxadd)
                {
                    if(KvadGetHexel(ptr, curj - gravx, curi - gravy)->tmp == 0
                    && KvadGetHexel(ptr, curj, curi)->stress == maximum)
                    {
                        KvadGetHexel(ptr, curj - gravx, curi - gravy)->tmp = curmat;
                        counter++;
                    }
                }
            }
            
            
            
            
        
            if(NeighbourCount(ptr, curj, curi, curmat) != 0)
                curj += lfx, curi += lfy;
                
            gx = rbx, gy = rby;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            
            
            turn = 6;
        }
        else
        {
            gx = lfx, gy = lfy;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            turn++;
        }
        
    }
    while(!(curj == j && curi == i) || turn < 6);
    int nextmin = 0;
    if(doprint) printf("waterloop\t%i\n", counter);
    while(counter > 0)
    {
        curj = j, curi = i;
        
        gx = grbx, gy = grby;
        
        RelToAbs(gx, gy, 2, &rbx, &rby);
        RelToAbs(gx, gy, -2, &lbx, &lby);
        RelToAbs(gx, gy, 1, &rfx, &rfy);
        RelToAbs(gx, gy, -1, &lfx, &lfy);
        
        turn = 0;
        
        do
        {
            
            if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
            && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
            || NeighbourCount(ptr, curj, curi, curmat) == 0)
            {
                if(KvadGetHexel(ptr, curj, curi)->stress == (minimum + nextmin) && counter > 0
                && KvadGetHexel(ptr, curj, curi)->tmp != 0)
                {
                    KvadGetHexel(ptr, curj, curi)->tmp = 0;
                    counter--;
                    deleted++;
                }
            
                if(NeighbourCount(ptr, curj, curi, curmat) != 0)
                    curj += lfx, curi += lfy;
                    
                gx = rbx, gy = rby;
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);
                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                
                
                
                turn = 6;
            }
            else
            {
                gx = lfx, gy = lfy;
                
                RelToAbs(gx, gy, 2, &rbx, &rby);
                RelToAbs(gx, gy, -2, &lbx, &lby);
                RelToAbs(gx, gy, 1, &rfx, &rfy);
                RelToAbs(gx, gy, -1, &lfx, &lfy);
                
                turn++;
            }
        }
        while(!(curj == j && curi == i) || turn < 6);
        
        if(minimum + nextmin > maximum)
        {
            counter--;
            if(doprint) printf("\033[0;30;41mERROR\n\033[0m");
        }
        
        nextmin++;
        
        
    }
    if(doprint) printf("deleted--\t%i\n", deleted);
    
    gx = grbx, gy = grby;
    
    RelToAbs(gx, gy, 2, &rbx, &rby);
    RelToAbs(gx, gy, -2, &lbx, &lby);
    RelToAbs(gx, gy, 1, &rfx, &rfy);
    RelToAbs(gx, gy, -1, &lfx, &lfy);
    
    turn = 0;
    
    do
    {
        
        if(NeighbourCount(ptr, curj + lfx, curi + lfy, curmat) < 6
        && KvadGetHexel(ptr, curj + lfx, curi + lfy)->mat == curmat
        || NeighbourCount(ptr, curj, curi, curmat) == 0)
        {
            if(KvadGetHexel(ptr, curj, curi)->stress >= 0)
            {
                KvadGetHexel(ptr, curj, curi)->stress = - 1 - abs(KvadGetHexel(ptr, curj, curi)->stress);
            }
        
            if(NeighbourCount(ptr, curj, curi, curmat) != 0)
                curj += lfx, curi += lfy;
                
            gx = rbx, gy = rby;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            
            
            turn = 6;
        }
        else
        {
            gx = lfx, gy = lfy;
            
            RelToAbs(gx, gy, 2, &rbx, &rby);
            RelToAbs(gx, gy, -2, &lbx, &lby);
            RelToAbs(gx, gy, 1, &rfx, &rfy);
            RelToAbs(gx, gy, -1, &lfx, &lfy);
            
            turn++;
        }
    }
    while(!(curj == j && curi == i) || turn < 6);

}

void Border(Kvad_t* ptr)
{
    for(int i = 0; i < ptr->height; i++)
    {
        KvadSetMat(ptr, 0, i, 0, 1);
        KvadSetMat(ptr, ptr->width - 1, i, 0, 1);
    }
    for(int j = 0; j < ptr->width; j++)
    {
        KvadSetMat(ptr, j, 0, 0, 1);
        KvadSetMat(ptr, j, ptr->height - 1, 0, 1);
    }
}

void SetGravity(int x, int y)
{
    gravx = x, gravy = y;
}

Node_t* MeatInitialize(Kvad_t* ptr, int x, int y, int n)
{
    if(doprintdebug) printf("MeatInitialize\n");
    Node_t *head = NULL;
    Node_t *tail = NULL;
    head = (Node_t*) malloc(sizeof(Node_t));
    // printf("ALLOCATED\n");
    if(head == NULL) printf("\n Head error");
    head->x = x, head->y = y;
    head->next = NULL;
    head->next2 = NULL;
    head->prev = NULL;
    head->energy = 100;
    head->stored = 0;
    int size = genomelist[n]->blueprint[0];
    
    
    Node_t *nodecur = head;
    Node_t *nodeprev = NULL;
    
    for(int i = 0; i < blueprintsize; i++)
    {
        switch (genomelist[n]->blueprint[i])
        {
        case 0:
            break;
        case 1:
            if(nodecur->next == NULL)
            {
                nodecur->next = (Node_t*) malloc(sizeof(Node_t));
                nodeprev = nodecur;
                
                nodecur = nodecur->next;
                
                nodecur->x = x;
                nodecur->y = y;
                nodecur->next = NULL;
                nodecur->next2 = NULL;
                nodecur->prev = nodeprev;
                nodecur->energy = 0;
                nodecur->stored = 0;
            }
            // grow next
            break;
        case 2:
            if(nodecur->next2 == NULL)
            {
                nodecur->next2 = (Node_t*) malloc(sizeof(Node_t));
                nodeprev = nodecur;
                
                nodecur = nodecur->next2;
                
                nodecur->x = x;
                nodecur->y = y;
                nodecur->next = NULL;
                nodecur->next2 = NULL;
                nodecur->prev = nodeprev;
                nodecur->energy = 0;
                nodecur->stored = 0;
                genomelist[n]->limbs++;
            }
            // grow next2
            break;
        case 3:
            if(nodecur->prev != NULL)
            {
                nodecur = nodecur->prev;
            }
            // go back
            break;
        
        default:
            break;
        }
    }
    
    MeatDraw(ptr, head);
    return head;
}

void MeatMove(Kvad_t* ptr, int n, int dx, int dy, int manual)
{
    if(doprintdebug) printf("MeatMove\n");
    Node_t *tail = NULL;
    Node_t *head = meatlist[n];
    Node_t *nodecur = head;
    Node_t *nodeprev = NULL;
    Node_t *nodenext2 = NULL;
    
    int prevx, prevy;
    
    prevx = head->x, prevy = head->y;
    
    MeatMoveHead(ptr, n, head, dx, dy, manual);
    
    while (nodecur != NULL)
    {
        if(nodecur->next2 != NULL)
        {
            // nodenext2 = nodecur->next2;
            // if(hdist(nodecur->x, nodecur->y, nodenext2->x, nodenext2->y) == 1 &&
            //         KvadGetHexel(ptr, nodecur->x + dx, nodecur->y + dy)->mat == 0)
            //     {
            //         prevx = nodecur->x;
            //         prevy = nodecur->y;
                    
            //         nodecur->x += dx;
            //         nodecur->y += dy;
            //     }
            //     else
            //     {
                    
            //         swap(&prevx, &nodecur->x);
            //         swap(&prevy, &nodecur->y); 
            //     }
        }
        
        nodecur = nodecur->next;
    }
}

void MeatMoveHead(Kvad_t* ptr, int n, Node_t *head, int dx, int dy, int manual)
{
    if(doprintdebug) printf("MeatMoveHead\n");
    Node_t *tail = NULL;
    Node_t *nodecur = head;
    Node_t *nodeprev = NULL;
    Node_t *nodenext2 = NULL;
    
    int delete = 0;
    if(manual)
    {
        delete = inpst.delete;
    }
    else
    {
        if(genomelist[n] != NULL)
        {
            delete = genomelist[n]->delete;
        }
        else
            delete = rand() % 100 > 98;
    }
    
    int prevx, prevy, t;
    
    prevx = head->x, prevy = head->y;
    
    if(genomelist[n] != NULL)
    {
        genomelist[n]->mathead = KvadGetHexel(ptr, head->x + dx, head->y + dy)->mat;
    }
    
    if((delete
     && (KvadGetHexel(ptr, head->x + dx, head->y + dy)->mat == 9
     || KvadGetHexel(ptr, head->x + dx, head->y + dy)->mat == 10
     || KvadGetHexel(ptr, head->x + dx, head->y + dy)->mat == 12
     || KvadGetHexel(ptr, head->x + dx, head->y + dy)->mat == 13
     )
     && 
     (((genomelist[n]->inpx != 0 || genomelist[n]->inpy != 0) && !manual)
     || ((inpst.vx != 0 || inpst.vy != 0) && manual))
    ) || KvadGetHexel(ptr, head->x + dx, head->y + dy)->mat == 0)
    {
        if(KvadGetHexel(ptr, head->x + dx, head->y + dy)->mat == 0)
        {
            // t = KvadGetHexel(ptr, head->x + dx, head->y + dy)->mat;
            //     KvadSetMat(ptr, head->x + dx, head->y + dy,
            //      KvadGetHexel(ptr, head->x, head->y)->mat);
            //     KvadSetMat(ptr, head->x, head->y, t);
            
            head->x += dx;
            head->y += dy;
            
            nodenext2 = head->next2;
        
            if(head->next2 != NULL
            && hdist(head->x, head->y, nodenext2->x, nodenext2->y) > 0)
            {
                MeatMoveHead(ptr, n, head->next2, 
                head->x - nodenext2->x, 
                head->y - nodenext2->y, manual);
            }
        }
        
        else if(meatlist[n]->stored == 0) 
        {
            
            if(KvadGetHexel(ptr, head->x + dx, head->y + dy)->mat != 10
            && KvadGetHexel(ptr, head->x + dx, head->y + dy)->mat != 12
            && KvadGetHexel(ptr, head->x + dx, head->y + dy)->mat != 13 || 1)
            {
                if(1)
                {
                    genomelist[n]->energy += KvadGetHexel(ptr, head->x + dx, head->y + dy)->nrj;
                    genomelist[n]->lifetime += KvadGetHexel(ptr, head->x + dx, head->y + dy)->nrj;
                    genomelist[n]->organics += KvadGetHexel(ptr, head->x + dx, head->y + dy)->org;
                    KvadGetHexel(ptr, head->x + dx, head->y + dy)->nrj = 0;
                }
                genomelist[n]->eaten++;
                meatlist[n]->stored = 0;
            }
            KvadSetBlob(ptr, head->x + dx, head->y + dy, 0, 0);
            
        }
        // if(doprint) printf("\nstored %i\n", head->stored);
        
    
        while (nodecur->next != NULL)
        {
            nodeprev = nodecur;
            nodecur = nodecur->next;
            
            if(hdist(nodeprev->x, nodeprev->y, nodecur->x, nodecur->y) > 1)
            {
                if(nodecur->next == NULL)
                {
                    if(meatlist[n]->stored != 0) 
                    {
                        KvadSetBlob(ptr, nodecur->x, nodecur->y, meatlist[n]->stored, 0);
                        meatlist[n]->stored = 0;
                    }
                }
            
                if(hdist(nodeprev->x, nodeprev->y, nodecur->x + dx, nodecur->y + dy) == 1 &&
                    KvadGetHexel(ptr, nodecur->x + dx, nodecur->y + dy)->mat == 0)
                {
                    prevx = nodecur->x;
                    prevy = nodecur->y;
                    
                    nodecur->x += dx;
                    nodecur->y += dy;
                    
                    nodenext2 = nodecur->next2;
        
                    if(nodecur->next2 != NULL
                    && hdist(nodecur->x, nodecur->y, nodenext2->x, nodenext2->y) > 0)
                    {
                        MeatMoveHead(ptr, n, nodecur->next2, 
                        nodecur->x - nodenext2->x, 
                        nodecur->y - nodenext2->y, manual);
                    }
                }
                else
                {
                    swap(&prevx, &nodecur->x);
                    swap(&prevy, &nodecur->y); 
                    
                    nodenext2 = nodecur->next2;
        
                    if(nodecur->next2 != NULL
                    && hdist(nodecur->x, nodecur->y, nodenext2->x, nodenext2->y) > 0)
                    {
                        MeatMoveHead(ptr, n, nodecur->next2, 
                        nodecur->x - nodenext2->x, 
                        nodecur->y - nodenext2->y, manual);
                    }
                }
            }
        }
    }
}

void MeatMoveTail(Kvad_t* ptr, int n, int manual, int which)
{
    if(doprintdebug) printf("MeatMoveTail\n");
    int pull = 0, push = 0, retract = 0, add = 0, dx = 0, dy = 0;
    if(manual)
    {
        inpst.vx = (inpst.right - inpst.left);
        inpst.vy = (inpst.down - inpst.up);
        
        int dz = 0, dn = 0;
        
        if(inpst.vy > 0)
            dz = 0, dn = 1;
        else
        {
            dz = inpst.vx;
            dn = (inpst.vx < 0) - (inpst.vy < 0);
        }
        
        dx = dz, dy = dn;
        
        pull = inpst.insertA;
        push = inpst.insertB;
        retract = inpst.jump;
    }
    else
    {
        int inpx, inpy;
        if(genomelist[n] != NULL)
        {
            pull = genomelist[n]->pull, push = genomelist[n]->push;
            inpx = genomelist[n]->inpx, inpy = genomelist[n]->inpy;
            add = genomelist[n]->add;
        }
        else
        {
            pull = 1, push = 0;
            inpx = 0, inpy = 0;
        }
        
        int dz = 0, dn = 0;
        
        if(inpy > 0)
            dz = 0, dn = 1;
        else
        {
            dz = inpx;
            dn = (inpx < 0) - (inpy < 0);
        }
        
        dx = dz, dy = dn;
        retract = genomelist[n]->retract;
        
    }
     
    
    Node_t *tail = meatlist[n];
    Node_t *head = meatlist[n];
    Node_t *nodecur = head;
    Node_t *nodeprev = NULL;
    
    int whichcounter = 0;
    
    while (nodecur != NULL)
    {
        if(whichcounter < which
        && nodecur->next2 != NULL)
        {
            nodecur = nodecur->next2;
            whichcounter++;
            // printf("next2 ");
        }
        else if(whichcounter < which
        && nodecur->next2 == NULL
        && nodecur->next != NULL)
        {
            nodecur = nodecur->next;
            // printf("next ");
        }
        else if(whichcounter >= which)
        {
            MeatMoveLimb(ptr, n, nodecur, manual);
            nodecur = NULL;
        }
        else nodecur = NULL;
        
        
    }
    
    // printf("end\n");
    
    // MeatMoveLimb(ptr, n, head0, manual);
}

void MeatMoveLimb(Kvad_t* ptr, int n, Node_t* head, int manual)
{
    if(doprintdebug) printf("MeatMoveLimb\n");
    int pull = 0, push = 0, retract = 0, add = 0, dx = 0, dy = 0;
    if(manual)
    {
        inpst.vx = (inpst.right - inpst.left);
        inpst.vy = (inpst.down - inpst.up);
        
        int dz = 0, dn = 0;
        
        if(inpst.vy > 0)
            dz = 0, dn = 1;
        else
        {
            dz = inpst.vx;
            dn = (inpst.vx < 0) - (inpst.vy < 0);
        }
        
        dx = dz, dy = dn;
        
        pull = inpst.insertA;
        push = inpst.insertB;
        retract = inpst.jump;
    }
    else
    {
        int inpx, inpy;
        if(genomelist[n] != NULL)
        {
            pull = genomelist[n]->pull, push = genomelist[n]->push;
            inpx = genomelist[n]->inpx, inpy = genomelist[n]->inpy;
            add = genomelist[n]->add;
        }
        else
        {
            pull = 1, push = 0;
            inpx = 0, inpy = 0;
        }
        
        int dz = 0, dn = 0;
        
        if(inpy > 0)
            dz = 0, dn = 1;
        else
        {
            dz = inpx;
            dn = (inpx < 0) - (inpy < 0);
        }
        
        dx = dz, dy = dn;
        retract = genomelist[n]->retract;
        
    }
     
    
    Node_t *tail = head;
    Node_t *nodecur = head;
    Node_t *nodeprev = NULL;
    Node_t *nodenext2 = NULL;
    
    while (nodecur->next != NULL)
    {
        nodecur = nodecur->next;
    }
    
    tail = nodecur;
    
    int prevx, prevy;
    int movehead = 1;
    int blowstrength = 1;
    int t;
    int displaced = 0;
    
    prevx = tail->x, prevy = tail->y;
    
    if(genomelist[n] != NULL)
    {
        genomelist[n]->mattail = KvadGetHexel(ptr, tail->x + dx, tail->y + dy)->mat;
    }
    
    if(KvadGetHexel(ptr, tail->x + dx, tail->y + dy)->mat == 0 && head != tail)
    {
        if(meatlist[n]->stored != 0 && 0) 
        {
            KvadSetBlob(ptr, tail->x + dx, tail->y + dy, meatlist[n]->stored, 0);
            meatlist[n]->stored = 0;
        }
        else
        {
            // t = KvadGetHexel(ptr, nodecur->x + dx, nodecur->y + dy)->mat;
            //     KvadSetMat(ptr, nodecur->x + dx, nodecur->y + dy,
            //      KvadGetHexel(ptr, nodecur->x, nodecur->y)->mat);
            //     KvadSetMat(ptr, nodecur->x, nodecur->y, t);
            tail->x += dx;
            tail->y += dy;
            // displaced = hdist(tail->x, tail->y, head->x, head->y);
            
            nodenext2 = tail->next2;
        
            if(tail->next2 != NULL
            && hdist(tail->x, tail->y, nodenext2->x, nodenext2->y) > 0)
            {
                MeatMoveHead(ptr, n, nodecur->next2, 
                tail->x - nodenext2->x, 
                tail->y - nodenext2->y, manual);
            }
        }
        movehead = 0;
    }
    
     if(push)
    {
        // KvadGetHexel(ptr, tail->x + dx, tail->y + dy)->dx = dx;
        // KvadGetHexel(ptr, tail->x + dx, tail->y + dy)->dy = dy;
        // KvadGetHexel(ptr, tail->x + dx, tail->y + dy)->val2 = blowstrength;
        
        // KvadGetHexel(ptr, tail->x + blowstrength * gravx, tail->y + blowstrength * gravy)->dx = -gravx;
        // KvadGetHexel(ptr, tail->x + blowstrength * gravx, tail->y + blowstrength * gravy)->dy = -gravy;
        // KvadGetHexel(ptr, tail->x + blowstrength * gravx, tail->y + blowstrength * gravy)->val2 = blowstrength;
        
    }
    
    while (nodecur->prev != head && nodecur->prev != NULL && head != tail)
    {
        nodeprev = nodecur;
        nodecur = nodecur->prev;
        
        if(hdist(nodeprev->x, nodeprev->y, nodecur->x, nodecur->y) > 1)
        {
            if(hdist(nodeprev->x, nodeprev->y, nodecur->x + dx, nodecur->y + dy) == 1 &&
                KvadGetHexel(ptr, nodecur->x + dx, nodecur->y + dy)->mat == 0)
            {
                if(
                nodecur->x - nodeprev->x != dx
                && nodecur->y - nodeprev->y != dy
                && nodecur->x - nodeprev->x + nodecur->y - nodeprev->y != dx + dy
                )
                    displaced++;
            
                prevx = nodecur->x;
                prevy = nodecur->y;
                
                // t = KvadGetHexel(ptr, nodecur->x + dx, nodecur->y + dy)->mat;
                // KvadSetMat(ptr, nodecur->x + dx, nodecur->y + dy,
                //  KvadGetHexel(ptr, nodecur->x, nodecur->y)->mat);
                // KvadSetMat(ptr, nodecur->x, nodecur->y, t);
                
                nodecur->x += dx;
                nodecur->y += dy;
                
                
                
                
                nodenext2 = nodecur->next2;
        
                if(nodecur->next2 != NULL
                && hdist(nodecur->x, nodecur->y, nodenext2->x, nodenext2->y) > 0)
                {
                    MeatMoveHead(ptr, n, nodecur->next2, 
                    nodecur->x - nodenext2->x, 
                    nodecur->y - nodenext2->y, manual);
                }
            }
            else
            {
                if(push)
                {
                    // KvadGetHexel(ptr, nodeprev->x + dx, nodeprev->y + dy)->dx = dx;
                    // KvadGetHexel(ptr, nodeprev->x + dx, nodeprev->y + dy)->dy = dy;
                    // KvadGetHexel(ptr, nodeprev->x + dx, nodeprev->y + dy)->val2 = blowstrength;
                    
                }
                
                // t = KvadGetHexel(ptr, prevx, prevy)->mat;
                // KvadSetMat(ptr, prevx, prevy,
                //  KvadGetHexel(ptr, nodecur->x, nodecur->y)->mat);
                // KvadSetMat(ptr, nodecur->x, nodecur->y, t);
                
                swap(&prevx, &nodecur->x);
                swap(&prevy, &nodecur->y); 
                // displaced--;
                
                nodenext2 = nodecur->next2;
        
                if(nodecur->next2 != NULL
                && hdist(nodecur->x, nodecur->y, nodenext2->x, nodenext2->y) > 0)
                {
                    MeatMoveHead(ptr, n, nodecur->next2, 
                    nodecur->x - nodenext2->x, 
                    nodecur->y - nodenext2->y, manual);
                }
            }
        }
    }
    if(nodecur->prev != NULL && head != tail)
    {
        nodeprev = nodecur;
        nodecur = nodecur->prev;
        
        int headdx = 0, headdy = 0;
        
        if(hdist(nodeprev->x, nodeprev->y, nodecur->x, nodecur->y) > 1)
        {
            movehead = 0;
            if(hdist(nodeprev->x, nodeprev->y, nodecur->x + dx, nodecur->y + dy) == 1
            && KvadGetHexel(ptr, nodecur->x + dx, nodecur->y + dy)->mat == 0)
            {
                prevx = nodecur->x;
                prevy = nodecur->y;
                
                // t = KvadGetHexel(ptr, nodecur->x + dx, nodecur->y + dy)->mat;
                // KvadSetMat(ptr, nodecur->x + dx, nodecur->y + dy,
                //  KvadGetHexel(ptr, nodecur->x, nodecur->y)->mat);
                // KvadSetMat(ptr, nodecur->x, nodecur->y, t);
                
                nodecur->x += dx;
                nodecur->y += dy;
                headdx = dx;
                headdy = dy;
                
                nodenext2 = nodecur->next2;
        
                if(nodecur->next2 != NULL
                && hdist(nodecur->x, nodecur->y, nodenext2->x, nodenext2->y) > 0)
                {
                    MeatMoveHead(ptr, n, nodecur->next2, 
                    nodecur->x - nodenext2->x, 
                    nodecur->y - nodenext2->y, manual);
                }
            }
            else
            {   
                // t = KvadGetHexel(ptr, prevx, prevy)->mat;
                // KvadSetMat(ptr, prevx, prevy,
                //  KvadGetHexel(ptr, nodecur->x, nodecur->y)->mat);
                // KvadSetMat(ptr, nodecur->x, nodecur->y, t);
                
                swap(&prevx, &nodecur->x);
                swap(&prevy, &nodecur->y); 
                headdx = nodecur->x - prevx;
                headdy = nodecur->y - prevy;
                
                nodenext2 = nodecur->next2;
        
                if(nodecur->next2 != NULL
                && hdist(nodecur->x, nodecur->y, nodenext2->x, nodenext2->y) > 0)
                {
                    MeatMoveHead(ptr, n, nodecur->next2, 
                    nodecur->x - nodenext2->x, 
                    nodecur->y - nodenext2->y, manual);
                }
            }
            MeatMoveHead(ptr, n, head, -headdx, -headdy, manual);
            displaced = 0;
        }
    }
    // if(doprint) printf("displaced %i\n", displaced);
    
    
    if((movehead == 1 && pull || displaced > 0 && genomelist[n]->airborne > 1) && head != tail)
    {
        MeatMoveHead(ptr, n, meatlist[n], -dx, -dy, manual);
    }
    
    if(displaced > 0) genomelist[n]->airborne += 1;
    else genomelist[n]->airborne = hmax(genomelist[n]->airborne - 1, 0);
    
    if(genomelist[n] != NULL)
    {
        genomelist[n]->deltax = tail->x - head->x;
        genomelist[n]->deltay = tail->y - head->y;
        // printf("\nDeltax: %i", genomelist[n]->deltax);
        // printf("\nDeltay: %i", genomelist[n]->deltay);
    }
    if(retract) MeatRetract(ptr, n, meatlist[n]);
    // if(add) 
    // {
    //     MeatAdd(ptr, n);
    //     genomelist[n]->lifetime -= 20;
    // }
    if(genomelist[n]->reproduce
     && genomelist[n]->energy >= 2000
     && genomelist[n]->organics >= 2000
      && (dx != 0 || dy != 0))
    {
        KvadSetBlob(ptr, tail->x, tail->y, 0, 0);
        KvadSetBlob(ptr, tail->x, tail->y, 12, n);
        genomelist[n]->energy -= 1000;
        genomelist[n]->organics -= 1000;
        genomelist[n]->happy += 10;
        genomelist[n]->lifetime += 0;
        MeatRetract(ptr, n, meatlist[n]);
        
        char buf[32];
        snprintf(buf, sizeof(buf), "genomes/genome_%d.txt", n);
        
        file_ptr = NULL;
        file_ptr = fopen(buf, "w");
    
        for(int i = 0; i < brainsize; i++)
        {
            fprintf(file_ptr, "%i\n",genomelist[n]->commandlist[i]);
            
        }
        fclose(file_ptr);
        
        snprintf(buf, sizeof(buf), "genomes/blueprint_%d.txt", n);
        
        file_ptr = NULL;
        file_ptr = fopen(buf, "w");
    
        for(int i = 0; i < blueprintsize; i++)
        {
            fprintf(file_ptr, "%i\n",genomelist[n]->blueprint[i]);
            
        }
        fclose(file_ptr);
    }
    genomelist[n]->energy -= MeatCountAll(ptr, meatlist[n]) / 5;
}

void MeatRetract(Kvad_t* ptr, int n, Node_t *head)
{
    if(doprintdebug) printf("MeatRetract\n");
    Node_t *tail = head;
    Node_t *nodecur = head;
    Node_t *nodeprev = NULL;
    Node_t *nodenext = NULL;
    
    while (nodecur->next != NULL)
    {
        nodecur = nodecur->next;
    }
    
    tail = nodecur;
    
    int prevx, prevy;
    
    prevx = tail->x, prevy = tail->y;
    
    while (nodecur != head && nodecur->prev != head && nodecur->prev != NULL)
    {
        
        nodeprev = nodecur;
        nodecur = nodecur->prev;
        nodeprev->x = nodecur->x;
        nodeprev->y = nodecur->y;
        if(nodeprev->next2 != NULL)
        {
            MeatRetract(ptr, n, nodeprev->next2);
            nodenext = nodeprev->next2;
            MeatMoveHead(ptr, n, nodeprev->next2, 
                nodeprev->x - nodenext->x, 
                nodeprev->y - nodenext->y, 0);
            
            nodenext->x = nodecur->x;
            nodenext->y = nodecur->y;
        }
    }
}

void MeatAdd(Kvad_t* ptr, int n)
{
    if(doprintdebug) printf("MeatAdd\n");
    Node_t *head = meatlist[n];
    Node_t *tail = NULL;
    Node_t *nodecur = head;
    
    
    while (nodecur->next != NULL)
    {
        KvadSetMat(ptr, nodecur->x, nodecur->y, 10, 1);
        nodecur = nodecur->next;
    }
    
    tail = nodecur;
    
    tail->next = (Node_t*) malloc(sizeof(Node_t));
        
    nodecur = tail->next;
    
    nodecur->x = tail->x;
    nodecur->y = tail->y;
    nodecur->next = NULL;
    nodecur->prev = tail;
    nodecur->energy = 0;
    nodecur->stored = 0;
}

void MeatDraw(Kvad_t* ptr, Node_t* head)
{
    if(doprintdebug) printf("MeatDraw\n");
    Node_t *tail = NULL;
    Node_t *nodecur = head;
    
    
    while (nodecur != NULL)
    {
        if(nodecur->next == NULL)
        {
            KvadSetMat(ptr, nodecur->x, nodecur->y, 13, 1);
            KvadGetHexel(ptr, nodecur->x, nodecur->y)->ded = 0;
            ChunkActivate(nodecur->x, nodecur->y);
        }
        else
        {
            KvadSetMat(ptr, nodecur->x, nodecur->y, 10, 1);
            KvadGetHexel(ptr, nodecur->x, nodecur->y)->ded = 0;
            ChunkActivate(nodecur->x, nodecur->y);
        }
        
        if(nodecur->next2 != NULL)
        {
            MeatDraw(ptr, nodecur->next2);
        }
        
        nodecur = nodecur->next;
    }
    KvadSetMat(ptr, head->x, head->y, 0, 1);
    KvadSetMat(ptr, head->x, head->y, 12, 1);
    ChunkActivate(head->x, head->y);
    
}

int MeatCountAll(Kvad_t* ptr, Node_t* head)
{
    if(doprintdebug) printf("MeatCountAll\n");
    Node_t *tail = NULL;
    Node_t *nodecur = head;
    int number = 0;
    
    
    while (nodecur != NULL)
    {
        number++;
        
        if(nodecur->next2 != NULL)
        {
            number += MeatCountAll(ptr, nodecur->next2);
        }
        
        nodecur = nodecur->next;
    }
    return number;
}

int MeatCountLimbs(Kvad_t* ptr, Node_t* head)
{
    if(doprintdebug) printf("MeatCountLimbs\n");
    Node_t *tail = NULL;
    Node_t *nodecur = head;
    int number = 1;
    
    
    while (nodecur != NULL)
    {
        
        if(nodecur->next2 != NULL)
        {
            number++;
            number += MeatCountAll(ptr, nodecur->next2);
        }
        
        nodecur = nodecur->next;
    }
    return number;
}

void MeatSetDead(Kvad_t* ptr, Node_t* head)
{
    if(doprintdebug) printf("MeatSetDead\n");
    Node_t *tail = NULL;
    Node_t *nodecur = head;
    Node_t *nodenext = NULL;
    
    while (nodecur != NULL)
    {
        KvadGetHexel(ptr, nodecur->x, nodecur->y)->ded = 1;
        
        if(nodecur->next2 != NULL)
        {
            MeatSetDead(ptr, nodecur->next2);
        }
        
        if(nodecur->next != NULL)
            nodenext = nodecur->next;
        else nodenext = NULL;
        
        nodecur = nodenext;
    }
}

void MeatClear(Kvad_t* ptr, Node_t* head)
{
    if(doprintdebug) printf("MeatClear\n");
    Node_t *tail = NULL;
    Node_t *nodecur = head;
    
    MeatSetDead(ptr, head);
    
    while (nodecur != NULL)
    {
        KvadSetMat(ptr, nodecur->x, nodecur->y, 0, 1);
        
        if(nodecur->next2 != NULL)
        {
            MeatClear(ptr, nodecur->next2);
        }
        
        nodecur = nodecur->next;
    }
}

void MeatFree(Node_t* head)
{
    if(doprintdebug) printf("MeatFree\n");
    Node_t *tail = NULL;
    Node_t *nodecur = head;
    Node_t *nodenext= NULL;
    
    // MeatSetDead(head);
    
    while (nodecur != NULL)
    {
        
        if(nodecur->next2 != NULL)
        {
            MeatFree(nodecur->next2);
        }
        if(nodecur->next != NULL)
            nodenext = nodecur->next;
        else nodenext = NULL;
        if(nodecur != NULL)
            free(nodecur);
        // printf("FREED\n");
        nodecur = nodenext;
    }
}

void MeatCheckUp(Kvad_t* ptr, int n, int kill)
{
    if(doprintdebug) printf("MeatCheckUp\n");
    Node_t *tail = NULL;
    Node_t *nodecur = meatlist[n];
    Node_t *nodeprev;
    Node_t *nodenext;
    
    // /*if(doprint)*/ printf("%i\n", active);
    
    if(meatlist[n] != NULL)
    {
        MeatSetDead(ptr, meatlist[n]);
        // MeatCheckLimb(ptr, meatlist[n], 1);
        if(meatlist[n]->next != NULL)
        {
            MeatCheckLimb(ptr, meatlist[n]->next, 1);
        }   
        if(meatlist[n]->next2 != NULL)
        {
            MeatCheckLimb(ptr, meatlist[n]->next2, 1);
        }   
        if((KvadGetHexel(ptr, meatlist[n]->x, meatlist[n]->y)->mat != 12
        && KvadGetHexel(ptr, meatlist[n]->x, meatlist[n]->y)->mat != 10
        && KvadGetHexel(ptr, meatlist[n]->x, meatlist[n]->y)->mat != 13) || kill)
        {
            if(doprint) printf("DEATH\n");
            
            if(!corpses) MeatClear(ptr, meatlist[n]);
            
            int xnew = meatlist[n]->x, ynew = meatlist[n]->y;
            
            if(kill && active < 81 && 0)
            {
                KvadSetBlob(ptr, xnew, ynew, 0, 0);
                KvadSetBlob(ptr, xnew, ynew, 12, n);
                if(doprint) printf("REBIRTH\n");
            }
            
            // MeatFree(meatlist[n]);
            
            free(meatlist[n]);
            BrainTerminate(n);
            if(doprint) printf("FREED LAST\n");
            meatlist[n] = NULL;
            
            
        }
    }
}

void MeatCheckLimb(Kvad_t* ptr, Node_t *head, int next)
{
    if(doprintdebug) printf("MeatCheckLimb\n");
    Node_t *tail = NULL;
    Node_t *nodecur = head;
    Node_t *nodeprev;
    Node_t *nodenext;
    
    int death_b = 0;
    while (nodecur != NULL)
    {
        
        nodenext = nodecur->next;
        if(KvadGetHexel(ptr, nodecur->x, nodecur->y)->mat != 10 &&
            KvadGetHexel(ptr, nodecur->x, nodecur->y)->mat != 12 &&
            KvadGetHexel(ptr, nodecur->x, nodecur->y)->mat != 13)
        {
            
            
            if(nodecur->prev != NULL)
            {
                nodeprev = nodecur->prev;
                if(next == 2 && nodecur == head)
                {
                    nodeprev->next2 = NULL;
                    if(doprint) printf("next 2\n");
                }
                else
                {
                    nodeprev->next = NULL;
                    if(doprint) printf("next 1\n");
                }
            }
            if(!corpses) MeatClear(ptr, nodecur); 
            MeatSetDead(ptr, nodecur);
            MeatFree(nodecur);
            
            nodenext = NULL;
            if(doprint) printf("FREED\n");
        }
        else if(nodecur->next2 != NULL)
        {
            MeatCheckLimb(ptr, nodecur->next2, 2);
        }
        nodecur = nodenext;
    }
    
}

void MeatListInitialize(Kvad_t* ptr)
{
    if(doprintdebug) printf("MeatListInitialize\n");
    meatlist = (Node_t**)malloc(meatlistsize * sizeof(Node_t*));
    genomelist = (Brain_t**)malloc(meatlistsize * sizeof(Node_t*));
    
    for(int i = 0; i < meatlistsize; i++)
    {
        meatlist[i] = NULL;
        genomelist[i] = NULL;
    }
    // meatlist[0] = MeatInitialize(ptr, 50, 50, 14);
    
    // if(startingpopulation) Populate(ptr);
}

void MeatListTerminate(Kvad_t* ptr)
{
    if(doprintdebug) printf("MeatListTerminate\n");
    for(int i = 0; i < meatlistsize; i++)
    {
        if(meatlist[i] != NULL)
        {
            MeatCheckUp(ptr, i, 1);
        }
        
    }
    free(meatlist);
    free(genomelist);
}

void MeatKillAll()
{
    if(doprintdebug) printf("MeatKillAll\n");
    for(int i = 0; i < meatlistsize; i++)
    {
        if(meatlist[i] != NULL)
        {
            MeatFree(meatlist[i]);
            meatlist[i] = NULL;
        }
        if(genomelist[i] != NULL)
        {
            BrainTerminate(i);
            genomelist[i] = NULL;
        }
        
    }
}

void MeatListUpdate(Kvad_t* ptr)
{
    if(doprintdebug) printf("MeatListUpdate\n");
    maxhappiness = 0;
    active = 0;
    
    for(int i = 0; i < meatlistsize; i++)
    {
        if(meatlist[i] != NULL)
        {
            MeatCheckUp(ptr, i, genomelist[i]->death);
        }
            
        if(genomelist[i] != NULL && meatlist[i] != NULL)
        {
            MeatClear(ptr, meatlist[i]);
            
            MeatMoveTail(ptr, i, manual, genomelist[i]->whichlimb);
            
            
            if(t == gravtime)
            {
                MeatMove(ptr, i, 
                GetGravX(ptr, meatlist[i]->x, meatlist[i]->y), 
                GetGravY(ptr, meatlist[i]->x, meatlist[i]->y), manual);
            }
            MeatDraw(ptr, meatlist[i]);
            KvadGetHexel(ptr, meatlist[i]->x, meatlist[i]->y)->nrj = genomelist[i]->energy;
            KvadGetHexel(ptr, meatlist[i]->x, meatlist[i]->y)->org = genomelist[i]->organics;
        }
        
        
    }
    for(int i = 0; i < meatlistsize; i++)
    {
        if(genomelist[i] != NULL && meatlist[i] != NULL)
        {
            genomelist[i]->limbs = MeatCountLimbs(ptr, meatlist[i]);
        }
        
    }
    
    for(int i = 0; i < meatlistsize; i++)
    {
        if(meatlist[i] != NULL)
        {
            active++;
            if(genomelist[i] != NULL && meatlist[i] != NULL)
            {
                if(!manual) BrainExecute(ptr, i);
            }
        }
    }
    
    if(active == 0)
    {
        if(startingpopulation) Populate(ptr);
    }
}

Brain_t* BrainInitialize(int n)
{
    if(doprintdebug) printf("BrainInitialize\n");
    Brain_t* gen = (Brain_t*) malloc(sizeof(Brain_t));
    
    if(gen == NULL) printf("brain error\n");
    // printf("BRAIN ALLOCATED\n");
    
    char buf[32];
    snprintf(buf, sizeof(buf), "genomes/genome_%d.txt", n);
    
    file_ptr = NULL;
    file_ptr = fopen(buf, "r");
    
    if(file_ptr == NULL) printf("file error\n");
    int random, chance = nonmutationchance;
    for(int i = 0; i < brainsize; i++)
        {
            gen->commandlist[i] = 0;
        }
    if(n != -1 && genomelist[n] != NULL)
    {
        for(int i = 0; i < brainsize; i++)
        {
            random = rand() % chance / (chance - 1);
            if(!mutations) random = 0;
            if(random == 1)
            {
                gen->commandlist[i] = mod(rand(), brainsize);
            }
            else if(genomelist[n] != NULL)
            {
                gen->commandlist[i] = genomelist[n]->commandlist[i];
            }
            else 
            {
                gen->commandlist[i] = 0;
            }
        }
    }
    else 
    if (file_ptr != NULL)
    {
        for(int i = 0; i < brainsize; i++)
        {
            fscanf(file_ptr, "%d", &integer);
            
            random = rand() % chance / (chance - 1);
            if(!mutations) random = 0;
            if(random == 1)
            {
                gen->commandlist[i] = mod(rand(), brainsize);
            }
            else
            {
                gen->commandlist[i] = mod(integer, brainsize);
            }
            
            
        }
        fclose(file_ptr);
        file_ptr = NULL;
    }
    else
    {
        for(int i = 0; i < brainsize; i++)
        {
            gen->commandlist[i] = mod(rand(), brainsize);
        }
    }
    
    if (file_ptr != NULL)
    {
        fclose(file_ptr);
    }
    
    
    
    gen->pointer = 0;
    gen->inpx = 0;
    gen->inpy = 0;
    gen->delete = 0;
    gen->pull = 1;
    gen->push = 0;
    gen->retract = 0;
    gen->add = 0;
    gen->happy = 0;
    gen->lastx = 0;
    gen->lastfullness = 0;
    gen->maxhappiness = 0;
    gen->eaten = 0;
    gen->drank = 0;
    gen->reproduce = 0;
    gen->lifetime = maxlife;
    gen->energy = 1000;
    gen->organics = 1000;
    gen->lenght = 0;
    gen->whichlimb = 0;
    gen->limbs = 0;
    gen->lastx = NULL;
    gen->memoryage = 0;
    gen->death = 0;
    gen->airborne = 0;
    
    
    snprintf(buf, sizeof(buf), "genomes/blueprint_%d.txt", n);
    
    file_ptr = NULL;
    file_ptr = fopen(buf, "r");
    chance = nonmutationchancebp;
    
    if(file_ptr == NULL) printf("file missing\n");
    for(int i = 0; i < blueprintsize; i++)
        {
            gen->blueprint[i] = 0;
        }
    if(n != -1 && genomelist[n] != NULL)
    {
        for(int i = 0; i < blueprintsize; i++)
        {
            random = rand() % chance / (chance - 1);
            if(!mutations) random = 0;
            if(random == 1)
            {
                gen->blueprint[i] = mod(rand(), 4);
            }
            else if(genomelist[n] != NULL)
            {
                gen->blueprint[i] = genomelist[n]->blueprint[i];
            }
            else 
            {
                gen->blueprint[i] = 0;
            }
        }
    }
    else 
    if (file_ptr != NULL)
    {
        for(int i = 0; i < blueprintsize; i++)
        {
            fscanf(file_ptr, "%d", &integer);
            
            random = rand() % chance / (chance - 1);
            if(!mutations) random = 0;
            if(random == 1)
            {
                gen->blueprint[i] = mod(rand(), 4);
            }
            else
            {
                gen->blueprint[i] = mod(integer, 4);
            }
            
            
        }
        fclose(file_ptr);
        file_ptr = NULL;
    }
    else
    {
        for(int i = 0; i < blueprintsize; i++)
        {
            gen->blueprint[i] = mod(rand(), 4);
        }
    }
    
    if (file_ptr != NULL)
    {
        fclose(file_ptr);
    }
    
    // gen->blueprint[0] = 1;
    // gen->blueprint[1] = 1;
    // gen->blueprint[2] = 1;
    // gen->blueprint[3] = 1;
    // gen->blueprint[4] = 1;
    // gen->blueprint[5] = 1;
    // gen->blueprint[6] = 1;
    // gen->blueprint[7] = 1;
    // gen->blueprint[8] = 1;
    // gen->blueprint[9] = 1;
    // gen->blueprint[10] = 2;
    
    UsageZero(gen);
    ArmorZero(gen);
    
    return gen;
    
}

void BrainTerminate(int n)
{
    if(doprintdebug) printf("BrainTerminate\n");
    if(genomelist[n] != NULL)
    {
        free(genomelist[n]);
        genomelist[n] = NULL;
    }
    // printf("BRAIN FREED\n");
}

void BrainExecute(Kvad_t* ptr, int n)
{
    int command = 0;
    int shift;
    Brain_t* gen;
    if(genomelist != NULL)
    {
    if(doprintdebug) printf("BrainExecute\n");
    gen = genomelist[n];
    command = gen->commandlist[gen->pointer];
    shift;
    gen->lifetime--;
    
    if(gen->armorlist[gen->pointer] != 0) isarmored = 1;
    loopsize++;
    
    if(gen->commandlist[gen->pointer] < 0 || gen->commandlist[gen->pointer] > brainsize - 1) printf("ERROR %i\n", gen->pointer);
    if(doprint) printf("\n%i\t%i\t%i\t%i",gen->pointer, command, gen->usagelist[gen->pointer], gen->armorlist[gen->pointer]); 
    
    if(command == 0)
    {
        shift = hdist(genomelist[n]->deltax, genomelist[n]->deltay, 0, 0) + 1;
        if (shift == 0) shift = 1;
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\ttail position", shift);
    }
    else if(command == 1)
    { 
        shift = genomelist[n]->mathead + 1;
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tmaterial at head", shift);
    }
    else if(command >= 2 && command <= 10)
    {
        shift = command - 1;
        
        int dir = command - 2;
        
        gen->inpx = mod(dir, 3) - 1;
        gen->inpy = hdiv(dir, 3) - 1;
        
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tmove tail", shift);
    }
    else if(command == 11 || command == 12)
    {
        shift = command - 10;
        
        int tgl = command - 11;
        
        gen->delete = tgl;
        
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tdelete toggle", shift);
    }
    else if(command == 13 || command == 14)
    {
        shift = command - 12;
        
        int tgl = command - 13;
        
        gen->pull = tgl;
        
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tpull toggle", shift);
    }
    else if(command == 15 || command == 16)
    {
        shift = command - 14;
        
        int tgl = command - 15;
        
        gen->push = tgl;
        
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tpush toggle", shift);
    }
    else if(command == 17)
    { 
        shift = genomelist[n]->mattail + 1;
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tmaterial at head", shift);
    }
    else if(command == 18)
    {
        int rad = 30, cntleft = 0, cntright = 0, cntup = 0, cntdown = 0, energy;
        shift = 1;
        
        for(int i = -rad; i <= rad; i++)
        {
            for(int j = -rad; j <= rad; j++)
            {
                if( i + j >= -rad && i + j <= rad)
                {
                    if(KvadGetHexel(ptr, meatlist[n]->x + j, meatlist[n]->y + i)->nrj > 0)
                    {
                        energy = KvadGetHexel(ptr, meatlist[n]->x + j, meatlist[n]->y + i)->nrj;
                        if(j < 0) cntleft += energy;
                        if(j > 0) cntright += energy;
                        if(i < 0 &&
                            j + i < 0) cntup += energy;
                        if(i > 0 &&
                            j + i > 0) cntdown += energy;
                    }
                }
            }
        }
        
        
        if(
            cntleft > cntup &&
            cntleft > cntright &&
            cntleft > cntdown) shift = 2, KvadGetHexel(ptr, meatlist[n]->x, meatlist[n]->y)->clr = 1;
        else if(
            cntup > cntleft &&
            cntup > cntright &&
            cntup > cntdown) shift = 3, KvadGetHexel(ptr, meatlist[n]->x, meatlist[n]->y)->clr = 2;
        else if(
            cntright > cntleft &&
            cntright > cntup &&
            cntright > cntdown) shift = 4, KvadGetHexel(ptr, meatlist[n]->x, meatlist[n]->y)->clr = 3;
        else if(
            cntdown > cntleft &&
            cntdown > cntright &&
            cntdown > cntup) shift = 5, KvadGetHexel(ptr, meatlist[n]->x, meatlist[n]->y)->clr = 4;
            
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tlook for energy", shift);
    }
    else if(command == 19)
    {
        int rad = 1, cntleft = 0, cntright = 0, cntup = 0, cntdown = 0;
        shift = 1;
        
        for(int i = -rad; i <= rad; i++)
        {
            for(int j = -rad; j <= rad; j++)
            {
                if( i + j >= -rad && i + j <= rad)
                {
                    if(KvadGetHexel(ptr, meatlist[n]->x + j, meatlist[n]->y + i)->mat == 0
                    && KvadGetHexel(ptr, meatlist[n]->x + j, meatlist[n]->y + i)->mat == 13
                    && KvadGetHexel(ptr, meatlist[n]->x + j, meatlist[n]->y + i)->mat == 12)
                    {
                        if(j < 0) cntleft++;
                        if(j > 0) cntright++;
                        if(i < 0 &&
                            j + i < 0) cntup++;
                        if(i > 0 &&
                            j + i > 0) cntdown++;
                    }
                }
            }
        }
        
        
        if(
            cntleft > cntup &&
            cntleft > cntright &&
            cntleft > cntdown) shift = 2;
        else if(
            cntup > cntleft &&
            cntup > cntright &&
            cntup > cntdown) shift = 3;
        else if(
            cntright > cntleft &&
            cntright > cntup &&
            cntright > cntdown) shift = 4;
        else if(
            cntdown > cntleft &&
            cntdown > cntright &&
            cntdown > cntup) shift = 5;
            
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tlook for space", shift);
    }
    else if(command == 20 || command == 21)
    {
        shift = command - 19;
        
        int tgl = command - 20;
        
        gen->retract = tgl;
        
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tretract toggle", shift);
    }
    else if(command == 22)
    {
        shift = 1;
        if(genomelist[n]->drank + genomelist[n]->eaten > 0) shift = 2;
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tfullness check", shift);
    }
    else if(command == 23 || command == 24)
    {
        shift = command - 22;
        int tgl = command - 23;
        gen->reproduce = tgl;
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\treproduce", shift);
    }
    else if(command == 25 || command == 26)
    {
        shift = command - 24;
        int tgl = command - 25;
        gen->add = tgl;
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tadd", shift);
    }
    else if(command == 27)
    {
        shift = gen->lenght + 1;
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tcheck lenght", shift);
    }
    else if(command == 28 || command == 29)
    {
        shift = command - 27;
        int ch = (command - 28) * 2 - 1;
        gen->whichlimb = cycle(gen->whichlimb, 0, gen->limbs - 1, ch);
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tswitch limb", shift);
    }
    else if(command == 30)
    {
        shift = gen->maxhappiness + 1;
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tcheck maxhappiness", shift);
    }
    else if(command == 31)
    {
        shift = abs(gen->energy) * 15 / maxenergy + 1;
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tcheck energy", shift);
    }
    else if(command == 32)
    {
        shift = abs(gen->lifetime) * 15 / maxlife + 1;
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tcheck lifetime", shift);
    }
    else if(command == 33)
    {
        int rad = 10, cntleft = 0, cntright = 0, cntup = 0, cntdown = 0;
        shift = 1;
        
        for(int i = -rad; i <= rad; i++)
        {
            for(int j = -rad; j <= rad; j++)
            {
                if( i + j >= -rad && i + j <= rad)
                {
                    if(KvadGetHexel(ptr, meatlist[n]->x + j, meatlist[n]->y + i)->ded == 0)
                    {
                        if(j < 0) cntleft++;
                        if(j > 0) cntright++;
                        if(i < 0 &&
                            j + i < 0) cntup++;
                        if(i > 0 &&
                            j + i > 0) cntdown++;
                    }
                }
            }
        }
        
        
        if(
            cntleft > cntup &&
            cntleft > cntright &&
            cntleft > cntdown) shift = 2;
        else if(
            cntup > cntleft &&
            cntup > cntright &&
            cntup > cntdown) shift = 3;
        else if(
            cntright > cntleft &&
            cntright > cntup &&
            cntright > cntdown) shift = 4;
        else if(
            cntdown > cntleft &&
            cntdown > cntright &&
            cntdown > cntup) shift = 5;
            
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tlook for life", shift);
    }
    else if(command == 34)
    {
        int rad = 1, cntleft = 0, cntright = 0, cntup = 0, cntdown = 0;
        shift = 1;
        
        for(int i = -rad; i <= rad; i++)
        {
            for(int j = -rad; j <= rad; j++)
            {
                if( i + j >= -rad && i + j <= rad)
                {
                    if(KvadGetHexel(ptr, meatlist[n]->x + j, meatlist[n]->y + i)->mat == 0
                    && KvadGetHexel(ptr, meatlist[n]->x + j, meatlist[n]->y + i)->mat == 13
                    && KvadGetHexel(ptr, meatlist[n]->x + j, meatlist[n]->y + i)->mat == 12)
                    {
                        if(j < 0) cntleft++;
                        if(j > 0) cntright++;
                        if(i < 0 &&
                            j + i < 0) cntup++;
                        if(i > 0 &&
                            j + i > 0) cntdown++;
                    }
                }
            }
        }
        
        
        shift = 1 + cntleft + cntright + cntup + cntdown;
            
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, shift);
        if(doprint) printf("\t%i\tis buried", shift);
    }
    else
    {
        gen->pointer = cycle(gen->pointer, 0, brainsize - 1, command);
        if(doprint) printf("\t%i", command);
    }
    
    // gen->usagelist[gen->poi./myprog arg1 arg2nter]++;
    
    // gen->happy = gen->eaten;
    // gen->energy = hmin(gen->energy + gen->eaten * 5, maxenergy);
    // gen->eaten = 0;
    
    if(gen->energy >= 2000) gen->reproduce = 1;
    gen->delete = 1;
    }
    if(gen->lifetime <= 0 || gen->energy <= 0)
    {
        int distance = abs(gen->lastx - meatlist[n]->x);
        if(gen->lastx == NULL) distance = 0;
        int excreted = (gen->lastfullness - gen->eaten - gen->drank);
        
        gen->lastx = meatlist[n]->x;
        gen->lastfullness = gen->eaten + gen->drank;
        
        gen->happy = 0;
        gen->death = 1;
    }
    
}

void UsageZero(Brain_t* gen)
{
    if(doprintdebug) printf("UsageZero\n");
    for(int i = 0; i < brainsize; i++)
    {
        gen->usagelist[i] = 0;
    }
}

void UsageFlip(Brain_t* gen)
{
    if(doprintdebug) printf("UsageFlip\n");
    for(int i = 0; i < brainsize; i++)
    {
        gen->usagelist[i] = - abs(gen->usagelist[i]);
    }
}

void ArmorZero(Brain_t* gen)
{
    if(doprintdebug) printf("ArmorZero\n");
    for(int i = 0; i < brainsize; i++)
    {
        gen->armorlist[i] = 0;
    }
}

void ArmorApply(Brain_t* gen)
{
    if(doprintdebug) printf("ArmorApply\n");
    int maximum = 0;
    for(int i = 0; i < brainsize; i++)
    {
        maximum = hmax(gen->usagelist[i], maximum);
    }
    
    for(int i = 0; i < brainsize; i++)
    {
        if(gen->usagelist[i] >= maximum - 1)
        {
            gen->armorlist[i] = hmin(gen->armorlist[i] + 1, usagearmormax);
        }
    }
}

void BrainRewire(Kvad_t* ptr, int n)
{
    if(doprintdebug) printf("BrainRewire\n");
    int dorewire = 0;
    
    Brain_t* gen = genomelist[n];
    for(int i = 0; i < brainsize; i++)
    {
        if(gen->usagelist[i] > rand() % (usagethreshold * (gen->happy + 1) * (gen->happy + 1))) dorewire = 1;
        else dorewire = 0;
        if(dorewire)
        {
            if(gen->armorlist[i] > 0)
                gen->armorlist[i] = hmax(gen->armorlist[i] - 1, 0);
            else
                gen->commandlist[i] = rand() % brainsize;
        }
    }
    
}

void Populate(Kvad_t* ptr)
{
    int counter = 0;
    int step;
    int random, nonchance = 20;
    
    KvadZero(ptr);
    
    step = 5;
    for(int i = border; i < ptr->height - border; i += step)
    {
        for(int j = border; j < ptr->width - border; j += step)
        {
            
            srand(time(NULL) + j * i * i);
            random = rand() % nonchance / (nonchance - 1);
            if(random)
            {
                // KvadSetBlob(ptr, j, i, 0, 1);
                KvadSetBlob(ptr, j, i, 9, 3);
            }
        }
    }
    step = 32;
    for(int i = border; i < ptr->height - border; i += step)
    {
        for(int j = border; j < ptr->width - border; j += step)
        {
            KvadSetBlob(ptr, j, i, 0, 5);
            KvadSetBlob(ptr, j, i, 12, counter);
            counter++;
        }
    }
    
}

Contour_t* ContourListInitialize()
{
    if(doprintdebug) printf("ContourListInitialize\n");
    
    Contour_t* head;
    head = (Contour_t*) malloc(sizeof(Contour_t));
    
    head->x = 1;
    head->y = 1;
    head->new = 1;
    head->next = NULL;
    head->prev = NULL;
    
    return head;
}

void ContourListTerminate(Contour_t* head)
{
    if(doprintdebug) printf("ContourListTerminate\n");
    
    Contour_t *concur = head;
    Contour_t *connext;
    
    while (concur != NULL)
    {
        connext = concur->next;
        free(concur);
        concur = connext;
    }
    
}

void ContourAdd(Kvad_t *ptr, int x, int y)
{
    if(doprintdebug) printf("ContourAdd\n");
    
    Contour_t *concur = maincontour;
    
    int newx = mod(x, ptr->width), newy = mod(y, ptr->height);
    
    while(concur != NULL)
    {
        if((concur->x == newx && concur->y == newy)) break;
        concur = concur->next;
    }
    if(concur == NULL)
    {
        Contour_t *con;
        con = (Contour_t*) malloc(sizeof(Contour_t));
        
        con->x = newx;
        con->y = newy;
        con->new = 1;
        con->next = NULL;
        
        concur = maincontour;
        
        while(concur->next != NULL)
        {
            concur = concur->next;
        }
        concur->next = con;
        con->prev = concur;
    }
    
}

void ContourDelete(int x, int y)
{
    if(doprintdebug) printf("ContourDelete\n");
    
    Contour_t *conprev = NULL;
    Contour_t *connext = NULL;
    Contour_t *concur = maincontour;
    
    while(concur != NULL)
    {
        if((concur->x == x && concur->y == y)) break;
        concur = concur->next;
    }
    if(concur != NULL)
    {
        
        conprev = concur->prev;
        connext = concur->next;
        
        if(conprev != NULL)
            conprev->next = connext;
        if(connext != NULL)
            connext->prev = conprev;
        free(concur);
    }
    
}

void ContourPrint(Kvad_t *ptr)
{
    if(doprintdebug) printf("ContourPrint\n");
    
    Contour_t *concur = maincontour;
    
    while(concur != NULL)
    {
        printf("%i\t%i\n", concur->x, concur->y);
        KvadGetHexel(ptr, concur->x, concur->y)->clr = 5;
        concur = concur->next;
    }
    
}

void ContourCheck(Kvad_t *ptr)
{
    if(doprintdebug) printf("ContourCheck\n");
    
    Contour_t *concur = maincontour->next;
    Contour_t *connext = NULL;
    int curmat;
    
    while(concur != NULL)
    {
        connext = concur->next;
        curmat = KvadGetHexel(ptr, concur->x, concur->y)->mat;
        if(concur->new == 1) concur->new = 0;
        if(NeighbourCount(ptr, concur->x, concur->y, curmat) == 6)
        {
            ContourDelete(concur->x, concur->y);
        }
        
        concur = connext;
    }
    
}

void ChunkKvadZero()
{
    for (int i = 0; i < chunkkvadsize; i++)
    {
        for (int j = 0; j < chunkkvadsize; j++)
        {
            chunkkvad[j][i] = 0;
        }
    }
    
}

void ChunkKvadUpdate()
{
    for (int i = 0; i < chunkkvadsize; i++)
    {
        for (int j = 0; j < chunkkvadsize; j++)
        {
            if(chunkkvad[j][i] == 1)
                chunkkvad[j][i] = 2;
                
            else if(chunkkvad[j][i] == 3)
            {
                int rad = 1;
                int isthere2 = 0;
                for(int i1 = - rad; i1 <=  + rad; i1++)
                {
                    for(int j1 = - rad; j1 <=  + rad; j1++)
                    {
                        if( i1 + j1 >=  - rad && i1 + j1 <= + rad)
                        {
                            if(chunkkvad[mod(j + j1, chunkkvadsize)][mod(i + i1, chunkkvadsize)] == 2
                            || chunkkvad[mod(j + j1, chunkkvadsize)][mod(i + i1, chunkkvadsize)] == 1)
                                isthere2 = 1;
                            
                        }
                        
                    }
                }
                if(isthere2 == 0)
                    chunkkvad[j][i] = 0;
            }
            else if(chunkkvad[j][i] == 0)
            {
                int rad = 1;
                int isthere2 = 0;
                for(int i1 = - rad; i1 <=  + rad; i1++)
                {
                    for(int j1 = - rad; j1 <=  + rad; j1++)
                    {
                        if( i1 + j1 >=  - rad && i1 + j1 <= + rad)
                        {
                            if(chunkkvad[mod(j + j1, chunkkvadsize)][mod(i + i1, chunkkvadsize)] == 2
                            || chunkkvad[mod(j + j1, chunkkvadsize)][mod(i + i1, chunkkvadsize)] == 1)
                                isthere2 = 1;
                            
                        }
                        
                    }
                }
                if(isthere2 == 1)
                    chunkkvad[j][i] = 3;
            }
        }
    }
    
}

void ChunkKvadPrint()
{
    printf("\n");
    for (int i = 0; i < chunkkvadsize; i++)
    {
        for (int j = 0; j < chunkkvadsize; j++)
        {
            printf("%i", chunkkvad[j][i]);
        }
        printf("\n");
    }
    
}

void ChunkActivate(int z, int n)
{   
    
    int rad = 1;
    for(int i = - rad; i <=  + rad; i++)
    {
        for(int j = - rad; j <=  + rad; j++)
        {
            if( i + j >=  - rad && i + j <= + rad)
            {
                if(chunkkvad[mod(hdiv(z, chunksize) + j, chunkkvadsize)][mod(hdiv(n, chunksize) + i, chunkkvadsize)] == 0
                || chunkkvad[mod(hdiv(z, chunksize) + j, chunkkvadsize)][mod(hdiv(n, chunksize) + i, chunkkvadsize)] == 3)
                    chunkkvad[mod(hdiv(z, chunksize) + j, chunkkvadsize)][mod(hdiv(n, chunksize) + i, chunkkvadsize)] = 1;
                
            }
            
        }
    }
    // rad = 2;
    // for(int i = - rad; i <=  + rad; i++)
    // {
    //     for(int j = - rad; j <=  + rad; j++)
    //     {
    //         if( i + j >=  - rad && i + j <= + rad)
    //         {
    //             if(chunkkvad[mod(hdiv(z, chunksize) + j, chunkkvadsize)][mod(hdiv(n, chunksize) + i, chunkkvadsize)] == 0)
    //                 chunkkvad[mod(hdiv(z, chunksize) + j, chunkkvadsize)][mod(hdiv(n, chunksize) + i, chunkkvadsize)] = 3;
                
    //         }
            
    //     }
    // }
}

int GetGravX(Kvad_t *ptr, int z, int n)
{
    int newx = mod(z, ptr->width) - gravcentx, newy = mod(n, ptr->height) - gravcenty;
    int newz = newx + newy;
    int newn = newx - newy;
    int newk = 2*newx + newy;
    int newl = newx + 2*newy;
    int newm = newx + newy;
    int dist = hdist(newx, newy, 0, 0);
    int distunit = 16;
    if(radial)
    {
        if((timer / gravtime) % (dist / distunit + 1) == 0 || gravfaloff == 0) 
        {
        if(newn <= 0 && newk > 0) return 0;
        if(newn >= 0 && newk < 0) return 0;
        if(newn > 0 && newl >= 0) return -1;
        if(newn < 0 && newl <= 0) return 1;
        if(newk >= 0 && newl < 0) return -1;
        if(newk <= 0 && newl > 0) return 1;
        }
        return 0;
    }
    else
    return gravx;
}

int GetGravY(Kvad_t *ptr, int z, int n)
{
    int newx = mod(z, ptr->width) - gravcentx, newy = mod(n, ptr->height) - gravcenty;
    int newz = newx + newy;
    int newn = newx - newy;
    int newk = 2*newx + newy;
    int newl = newx + 2*newy;
    int newm = newx + newy;
    int dist = hdist(newx, newy, 0, 0);
    int distunit = 16;
    if(radial)
    {
        if((timer / gravtime) % (dist / distunit + 1) == 0 || gravfaloff == 0) 
        {
        if(newn <= 0 && newk > 0) return -1;
        if(newn >= 0 && newk < 0) return 1;
        if(newn > 0 && newl >= 0) return 0;
        if(newn < 0 && newl <= 0) return 0;
        if(newk >= 0 && newl < 0) return 1;
        if(newk <= 0 && newl > 0) return -1;
        }
        return 0;
    }
    else
    return gravy;
}

void KvadDownload(Kvad_t *ptr, int n)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "kvads/kvad_%d.txt", n);
    
    file_ptr = NULL;
    file_ptr = fopen(buf, "w");

    for(int i = 0; i < ptr->height; i++)
    {
        for(int j = 0; j < ptr->width; j++)
        {
            fprintf(file_ptr, "% 2i", KvadGetHexel(ptr, j, i)->mat );
        }
        fprintf(file_ptr, "\n");
    }
    fclose(file_ptr);
}

void KvadUpload(Kvad_t *ptr, int n)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "kvads/kvad_%d.txt", n);
    
    file_ptr = NULL;
    file_ptr = fopen(buf, "r");
    if(file_ptr != NULL)
    {
        for(int i = 0; i < ptr->height; i++)
        {
            for(int j = 0; j < ptr->width; j++)
            {
                integer = 0;
                fscanf(file_ptr, "%2i", &integer);
                KvadSetMat(ptr, j, i, integer, 1);
            }
        }
        fclose(file_ptr);
    }
    else printf("file missing");
}

void KvadGenerate(Kvad_t *ptr, int n)
{
    KvadZero(ptr);
    // KvadSetBlob(ptr, center, center, 8, 30);
    // KvadSetBlob(ptr, center, center, 5, 100);
    int step = 5, size;
    int random, nonchance = 10;
    int x, y, ang;
    int agents = 5, length = 1000;
    int mult = 1;
    for (int m = 0; m < 100 * mult; m++)
    {
        x = center, y = center, ang = 0;
        for (int i = 0; i < 100 * mult; i++)
        {
            step = rand() % 1;
            KvadSetBlob(ptr, x, y, 11, step);
            x += Yrot[ang] * (step + 1);
            y += Yrot[mod(ang - 2, 6)] * (step + 1);
            ang = cycle(ang, 0, 5, rand() % 3 - 1);
        }
    }
    for (int m = 0; m < 5 * mult; m++)
    {
        x = center, y = center, ang = 0;
        for (int i = 0; i < 150 * mult; i++)
        {
            step = rand() % 1 + 1;
            KvadSetBlob(ptr, x, y, 3, step);
            x += Yrot[ang] * (step + 1);
            y += Yrot[mod(ang - 2, 6)] * (step + 1);
            ang = cycle(ang, 0, 5, rand() % 3 - 1);
        }
    }
    // for (int m = 0; m < agents; m++)
    // {
    //     x = center, y = center, ang = 0;
    //     for (int i = 0; i < length; i++)
    //     {
    //         step = rand() % 3;
    //         KvadSetBlob(ptr, x, y, 4, step);
    //         x += Yrot[ang] * (step + 1);
    //         y += Yrot[mod(ang - 2, 6)] * (step + 1);
    //         ang = cycle(ang, 0, 5, rand() % 3 - 1);
    //     }
    // }
    // for (int m = 0; m < agents; m++)
    // {
    //     x = center, y = center, ang = 0;
    //     for (int i = 0; i < length; i++)
    //     {
    //         step = rand() % 4;
    //         KvadSetBlob(ptr, x, y, 3, step);
    //         x += Yrot[ang] * (step + 1);
    //         y += Yrot[mod(ang - 2, 6)] * (step + 1);
    //         ang = cycle(ang, 0, 5, rand() % 3 - 1);
    //     }
    // }
    // for (int m = 0; m < 1; m++)
    // {
    //     x = center, y = center, ang = 0;
    //     for (int i = 0; i < 1000; i++)
    //     {
    //         step = rand() % 1;
    //         KvadSetBlob(ptr, x, y, 0, step);
    //         KvadSetBlob(ptr, x, y, 9, step);
    //         x += Yrot[ang] * (step + 1);
    //         y += Yrot[mod(ang - 2, 6)] * (step + 1);
    //         ang = cycle(ang, 0, 5, rand() % 3 - 1);
    //     }
    // }
}