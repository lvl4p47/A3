#ifndef HEXAGONS_H_INCLUDED
#define HEXAGONS_H_INCLUDED

#include "input.h"

Kvad_t* KvadInitialize(int width, int height);

void KvadTerminate(Kvad_t*);

void KvadZero(Kvad_t* ptr);

void KvadSetMat(Kvad_t* ptr, int z, int n, int value);

Cell_t* KvadGetHexel(Kvad_t* ptr, int z, int n);

void KvadSetBlob(Kvad_t* ptr, int z, int n, int value, int rad);

void KvadUpdate(Kvad_t* ptr);

int NeighbourCount(Kvad_t* ptr, int z, int n);

int ReflectionAngle(Kvad_t* ptr, int z, int n, int val, int dirz, int diry);

void Repulsion(Kvad_t* ptr, int z, int n, int val, int* dz, int* dn);

#endif
