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
    int mat, tmp, fld, fld2, val1, val2, dx, dy, st8, dns, clr, stress, v1t, v2t, pwr, flow[6], ded;
    
} Cell_t;

typedef struct
{
    int width, height;
    Cell_t **arr;
} Kvad_t;

typedef struct
{
    int x, y, dx, dy, energy, stored;
    struct Node_t *next;
    struct Node_t *prev;
    struct Node_t *next2;
} Node_t;


typedef struct
{
    int commandlist[256];
    int usagelist[256];
    int armorlist[256];
    int eaten, drank;
    int inpx, inpy, delete, pull, push, retract, add, deltax, deltay, mathead, mattail, reproduce;
    int pointer;
    int lastx, lastfullness, lifetime, energy, lenght;
    int happy, maxhappiness, memoryage;
    int whichlimb, limbs;
    int death, airborne;
} Brain_t;

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

void PhysicsUpdate2(Kvad_t* ptr);

void PositionsUpdate(Kvad_t* ptr);

void KvadUpdate(Kvad_t* ptr);

void EntityCollision(Kvad_t* ptr, Entity_t* p_e);

int NeighbourCount(Kvad_t* ptr, int z, int n, int val);

void Repulsion(Kvad_t* ptr, int z, int n, int* dz, int* dn, int empty_st8);

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

void ContourPressure(Kvad_t* ptr, int j, int i);

void ContourMove(Kvad_t* ptr, int j, int i);

void ContourMoveSand(Kvad_t* ptr, int j, int i);

void ContourMoveSolid(Kvad_t* ptr, int j, int i);

void Border(Kvad_t* ptr);

void SetGravity(int x, int y);

Node_t* MeatInitialize(Kvad_t* ptr, int x, int y, int size);

void MeatCheckUp(Kvad_t* ptr, int n, int kill);

void MeatCheckLimb(Kvad_t* ptr, Node_t *head, int next);

void MeatMove(Kvad_t* ptr, int n, int dx, int dy, int manual);

void MeatMoveHead(Kvad_t* ptr, int n, Node_t *head, int dx, int dy, int manual);

void MeatMoveTail(Kvad_t* ptr, int n, int manual, int which);

void MeatMoveLimb(Kvad_t* ptr, int n, Node_t* head, int manual);

void MeatRetract(Kvad_t* ptr, Node_t *head);

void MeatAdd(Kvad_t* ptr, int n);

void MeatDraw(Kvad_t* ptr, Node_t* head);

void MeatSetDead(Kvad_t* ptr, Node_t* head);

void MeatClear(Kvad_t* ptr, Node_t* head);

void MeatFree(Kvad_t* ptr, Node_t* head);

void MeatListInitialize(Kvad_t* ptr);

void MeatListTerminate(Kvad_t* ptr);

void MeatListUpdate(Kvad_t* ptr);

Brain_t* BrainInitialize(int n);

void BrainTerminate(int n);

void BrainExecute(Kvad_t* ptr, int n);

void UsageZero(Brain_t* gen);

void UsageFlip(Brain_t* gen);

void ArmorZero(Brain_t* gen);

void BrainRewire(Kvad_t* ptr, int n);

#endif
