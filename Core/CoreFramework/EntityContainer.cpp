#include "EntityContainer.h"

#include "Game.h"
#include "Level.h"

EntityContainer::EntityContainer(EntityContainer&& other) noexcept
{
    Entities = other.Entities;
    other.Entities.clear();
}

ComponentManager& EntityContainer::GetComponentManager() const
{
    return Game::Get().GetLevel()->GetComponentManager();
}
