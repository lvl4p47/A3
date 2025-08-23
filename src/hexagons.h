#ifndef HEXAGONS_H_INCLUDED
#define HEXAGONS_H_INCLUDED

#include "entity.h"

extern const int mat_amount;

extern int **st8_dns_clr;

typedef struct
{
    int flag;
    int neighbors[6];
} RulesConditions_t;

typedef struct
{
    int mat_to, num;
    RulesConditions_t **req;// * num
} RulesToMat_t;

typedef struct
{
    int mat_from;
    RulesToMat_t **tomat;// * mat_amount
} RulesFromMat_t;

typedef struct
{
    RulesFromMat_t **frommat;// * mat_amount
} Rules_t;

typedef struct
{
    int mat, tmp, fld, fld2, val1, val2, dx, dy, st8, dns, clr, stress, v1t, v2t;
    
} Cell_t;

typedef struct
{
    int width, height;
    Cell_t **arr;
} Kvad_t;

extern Rules_t *RULES;

void HexagonsInitialize();

void HexagonsTerminate();

void RulesInitialize();

void RulesTerminate();

void RulesAdd(int from, int to, int flag, int n1, int n2, int n3, int n4, int n5, int n6);

void RulesChange(int from, int to, int num, int flag, int n1, int n2, int n3, int n4, int n5, int n6);

void RulesRemove(int from, int to);

Kvad_t* KvadInitialize(int width, int height);

void KvadTerminate(Kvad_t*);

void KvadZero(Kvad_t* ptr);

void KvadSetMat(Kvad_t* ptr, int z, int n, int value);

Cell_t* KvadGetHexel(Kvad_t* ptr, int z, int n);

void KvadSwapCells(Kvad_t* ptr, int z, int n, int dz, int dn);

void KvadPartSwapCells(Kvad_t* ptr, int z, int n, int dz, int dn);

void KvadSetBlob(Kvad_t* ptr, int z, int n, int value, int rad);

void WaveUpdate(Kvad_t* ptr);

void SolidUpdate(Kvad_t* ptr);

void AudioCount(Kvad_t* ptr, int b_pause);

void AudioUpdate(Kvad_t* ptr, int b_pause);

void PhysicsUpdate(Kvad_t* ptr);

void KvadUpdate(Kvad_t* ptr);

void EntityCollision(Kvad_t* ptr, Entity_t* p_e);

int NeighbourCount(Kvad_t* ptr, int z, int n, int val);

void Repulsion(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn, int empty_st8);

void RelToAbs(int fx, int fy, int rel, int* dz, int* dn);

void ForceDirt(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void ForceSand(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void ForceRope(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void ForceLiquid(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void ForceIce(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void ForceGas(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void ForceRock(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void ForceRigid(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void ForceViscous(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void Border(Kvad_t* ptr);

#endif
