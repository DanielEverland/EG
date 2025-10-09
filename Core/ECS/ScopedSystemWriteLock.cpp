#include "ScopedSystemWriteLock.h"

#include "SystemMutexCollection.h"

ScopedSystemWriteLock::ScopedSystemWriteLock(const System *toLock) : LockedSystem(toLock)
{
    SystemMutexCollection::Get().Lock(LockedSystem);
}

ScopedSystemWriteLock::~ScopedSystemWriteLock()
{
    SystemMutexCollection::Get().Unlock(LockedSystem);
}
