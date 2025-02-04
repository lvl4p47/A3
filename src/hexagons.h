#ifndef HEXAGONS_H_INCLUDED
#define HEXAGONS_H_INCLUDED

#include "input.h"

Kvad_t* KvadInitialize(int width, int height);

void KvadTerminate(Kvad_t*);

void KvadZero(Kvad_t* ptr);

void KvadSetHexel(Kvad_t* ptr, int z, int n, int value);

void KvadSetBlob(Kvad_t* ptr, int z, int n, int value, int rad);

void KvadUpdate(Kvad_t* ptr);

int CountNeighbour(Kvad_t* ptr, int z, int n);

#endif
