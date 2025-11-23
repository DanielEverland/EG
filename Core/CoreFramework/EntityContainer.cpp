#include "EntityContainer.h"

#include "Game.h"
#include "Level.h"

ComponentManager& EntityContainer::GetComponentManager() const
{
    return Game::Get().GetLevel()->GetComponentManager();
}
