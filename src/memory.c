#include "cat/memory.h"

#include <stdlib.h>

void catRaiiFree(void* ptr)
{
    // The attribute gives a reference to the variable
    // but in the format of a pointer; Thus we have to recast its proper value (void**)
    void** p = (void**)ptr;
    free(*p);
}
