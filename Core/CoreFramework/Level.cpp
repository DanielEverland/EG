#include "Level.h"
#include "Application.h"
#include "Game.h"
#include "ECS/EntityFactory.h"

Entity Level::CreateEntity() const
{
    return Game::Get().GetNextEntity();
}

Entity Level::CreateEntity(const std::string& templateId) const
{
    Entity entity = Game::Get().GetNextEntity();
    Application::Get().GetEntityFactory()->PopulateEntity(entity, templateId);
    return entity;
}

Entity Level::CreateEntity(const std::string& templateId, const IntVector2D& position) const
{
    Entity entity = Game::Get().GetNextEntity();
    Application::Get().GetEntityFactory()->PopulateEntity(entity, templateId, position);
    return entity;
}

void Level::DestroyEntity(Entity entity)
{
    Components.RemoveEntity(entity);
}
