#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "input.h"

enum entity
{
    scale = 4
};

typedef struct
{
    int z, n, subz, subn, magn, angle;
    int fuel, health, oxygen;
} Entity_t;

extern Entity_t* e1;

Entity_t* EntityInitialize(z, n);

void EntityTerminate(Entity_t* p_e);

void EntitySystemInitialize();

void EntitySystemTerminate();

void EntityTeleport(Entity_t* p_e, int z, int n);

void EntityUpdate(Entity_t* p_e);

#endif
