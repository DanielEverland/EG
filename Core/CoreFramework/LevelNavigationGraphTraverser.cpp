#include "LevelNavigationGraphTraverser.h"

#include "Level.h"
#include "Components/NavigableComponent.h"

LevelNavigationGraphTraverser::LevelNavigationGraphTraverser(std::shared_ptr<::Level> level) : Level(level)
{
}

bool LevelNavigationGraphTraverser::IsValidMove(Entity entity, const IntVector& from, const IntVector& to)
{
    return Level->IsValidMove(entity, from, to);
}

bool LevelNavigationGraphTraverser::TryGetTraversalDifficulty(const IntVector& from, const IntVector& to, uint8_t& outTraversalDifficulty)
{
    const EntityContainer& entityContainer = Level->GetEntitiesAtPosition(to);
    NavigableComponent* navigableComponent = entityContainer.TryGetFirstComponent<NavigableComponent>();

    if (navigableComponent == nullptr)
    {
        return false;
    }

    outTraversalDifficulty = navigableComponent->TraversalDifficulty;
    return true;
}
