#include "SystemMutexCollection.h"

#include "CoreFramework/Game.h"

SystemMutexCollection& SystemMutexCollection::Get()
{
    return Game::Get().MutexCollection;
}

void SystemMutexCollection::Lock(const System *system)
{
    std::unordered_set<size_t>& mutexSet = ComponentsToLock[system];
    ComponentManager& componentManager = Game::Get().GetLevel()->GetComponentManager();

    const auto& components = componentManager.ComponentContainers;

    for (const auto& element : components)
    {
        if (mutexSet.contains(element.first))
        {
            element.second->LockWritable();
        }
    }
}

void SystemMutexCollection::Unlock(const System *system)
{
    std::unordered_set<size_t>& mutexSet = ComponentsToLock[system];
    ComponentManager& componentManager = Game::Get().GetLevel()->GetComponentManager();

    const auto& components = componentManager.ComponentContainers;

    for (const auto& element : components)
    {
        if (mutexSet.contains(element.first))
        {
            element.second->UnlockWritable();
        }
    }
}
