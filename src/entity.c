#include "entity.h"

Entity_t* e1;

Entity_t* EntityInitialize(z, n)
{
    Entity_t* p_e = (Entity_t*)malloc(sizeof(Entity_t));
    p_e->z = z;
    p_e->n = n;
    p_e->subz = 0;
    p_e->subn = 0;
    p_e->magn = 8;
    p_e->fuel = 20;
    
    return p_e;
}

void EntityTerminate(Entity_t* p_e)
{
    free(p_e);
}

void EntitySystemInitialize()
{
    e1 = EntityInitialize(-48, -48);
}

void EntitySystemTerminate()
{
    EntityTerminate(e1);
}

void EntityUpdate(Entity_t* p_e)
{

    
    
    
}
