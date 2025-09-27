#include "GenericHandle.h"

GenericHandle::GenericHandle()
{
    static uint32_t NextId = 0;
    Id = NextId++;
}
