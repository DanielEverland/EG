#include "Level.h"
#include "Game.h"

Entity Level::CreateEntity() const
{
    return Game::Get().GetNextEntity();
}

Entity Level::CreateEntity(const std::string& templateId) const
{
    Entity entity = Game::Get().GetNextEntity();

    
    
    return entity;
}

void Level::DestroyEntity(Entity entity)
{
    Components.RemoveEntity(entity);
}
