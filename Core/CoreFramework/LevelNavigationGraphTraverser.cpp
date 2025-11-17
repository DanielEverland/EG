#include "LevelNavigationGraphTraverser.h"

#include "Level.h"
#include "Components/NavigableComponent.h"

LevelNavigationGraphTraverser::LevelNavigationGraphTraverser(std::shared_ptr<::Level> level) : Level(level)
{
}

bool LevelNavigationGraphTraverser::DoesWorldPositionExist(const IntVector& worldPosition)
{
    return Level->DoesWorldPositionExist(worldPosition);
}

bool LevelNavigationGraphTraverser::TryGetTraversalDifficulty(const IntVector& from, const IntVector& to, uint8_t& outTraversalDifficulty)
{
    auto entities = Level->GetEntitiesAtPosition(to);
    NavigableComponent* navigableComponent = nullptr;
    for (Entity neighbor : entities)
    {
        navigableComponent = Level->GetComponentManager().TryGetComponent<NavigableComponent>(neighbor);
                
        // TODO: Handle multiple entities with navigable component
        if (navigableComponent != nullptr)
            break;
    }

    if (navigableComponent == nullptr)
    {
        return false;
    }

    outTraversalDifficulty = navigableComponent->TraversalDifficulty;
    return true;
}
