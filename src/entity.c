#include "entity.h"

Entity_t* e1;

Entity_t* EntityInitialize(z, n)
{
    Entity_t* p_e = (Entity_t*)malloc(sizeof(Entity_t));
    p_e->z = z;
    p_e->n = n;
    p_e->subz = 0;
    p_e->subn = 0;
    p_e->magn = scale;
    p_e->fuel = 48;
    p_e->angle = 0;
    p_e->health = 100;
    p_e->oxygen = 100;
    
    
    return p_e;
}

void EntityTerminate(Entity_t* p_e)
{
    free(p_e);
}

void EntitySystemInitialize()
{
    e1 = EntityInitialize(side / 2 - 100, side / 2 - 100);
}

void EntitySystemTerminate()
{
    EntityTerminate(e1);
}

void EntityTeleport(Entity_t* p_e, int z, int n)
{
    p_e->z = z;
    p_e->n = n;
}

void EntityUpdate(Entity_t* p_e)
{
    
}


