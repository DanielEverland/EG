#include "Level.h"
#include "Game.h"

Entity Level::CreateEntity() const
{
    return Game::Get().GetNextEntity();
}

void Level::DestroyEntity(Entity entity)
{
    Components.RemoveEntity(entity);
}
