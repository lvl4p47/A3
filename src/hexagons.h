#ifndef HEXAGONS_H_INCLUDED
#define HEXAGONS_H_INCLUDED

#include "input.h"

void RulesInitialize();

void RulesTerminate();

Kvad_t* KvadInitialize(int width, int height);

void KvadTerminate(Kvad_t*);

void KvadZero(Kvad_t* ptr);

void KvadSetMat(Kvad_t* ptr, int z, int n, int value);

Cell_t* KvadGetHexel(Kvad_t* ptr, int z, int n);

void KvadSetBlob(Kvad_t* ptr, int z, int n, int value, int rad);

void WaveUpdate(Kvad_t* ptr);

void SolidUpdate(Kvad_t* ptr);

void GasUpdate(Kvad_t* ptr);

void SandUpdate(Kvad_t* ptr);

void DirtUpdate(Kvad_t* ptr);

void RopeUpdate(Kvad_t* ptr);

void LiquidUpdate(Kvad_t* ptr);

void IceUpdate(Kvad_t* ptr);

void KvadUpdate(Kvad_t* ptr);

int NeighbourCount(Kvad_t* ptr, int z, int n, int val);

void Repulsion(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn, int empty_st8);

void RelToAbs(Kvad_t* ptr, int fx, int fy, int rel, int* dz, int* dn);

void ForceDirt(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void ForceSand(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void ForceTension(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void ForceLiquid(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void ForceIce(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void ForceGas(Kvad_t* ptr, int z, int n, int fx, int fy, int* dz, int* dn);

void Border(Kvad_t* ptr);

#endif
