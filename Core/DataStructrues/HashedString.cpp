#include "HashedString.h"

#include <cassert>

HashedString::operator std::string() const
{
    assert(Idx != UINT32_MAX);
    return Cache[Idx]; 
}
