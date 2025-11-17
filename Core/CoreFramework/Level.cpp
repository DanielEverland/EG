#include "Level.h"
#include "Application.h"
#include "Game.h"
#include "Components/TextureRendererComponent.h"
#include "ECS/EntityFactory.h"
#include "GameplayMessages/GameplayMessages.h"
#include "Levels/MapChunk.h"

Level::Level() : Components({ })
{
    GameplayMessages::Get().SubscribeMessage(HashedString("EntityLocationChanged"), std::bind(&Level::OnEntityLocationChanged, this, std::placeholders::_1));
}

void Level::LoadChunk(const IntVector& chunkPos, std::shared_ptr<MapChunk> chunk)
{
    if (!Chunks.contains(chunkPos))
    {
        Chunks.emplace(chunkPos, Chunk());
    }
    
    for (int x = 0; x < WorldPositionUtility::ChunkWidth; ++x)
    {
        for (int y = 0; y < WorldPositionUtility::ChunkHeight; ++y)
        {
            const MapCellInfo& cellInfo = chunk->Terrain[x + y * WorldPositionUtility::ChunkWidth];
            IntVector entityWorldPos = WorldPositionUtility::ChunkSpaceToWorldPosition(chunkPos, IntVector2D(x, y));
            Entity entity = CreateEntity(cellInfo.CellTypeName, IntVector2D(entityWorldPos.X, entityWorldPos.Y));
                
            auto& renderer = Game::Get().GetLevel()->GetComponentManager().GetComponentChecked<TextureRendererComponent>(entity);
            renderer.TextureName = cellInfo.TextureName;
        }
    }
}

bool Level::DoesWorldPositionExist(const IntVector& worldPosition)
{
    const IntVector chunkPos = WorldPositionUtility::WorldPositionToChunkPosition(worldPosition);
    return Chunks.contains(chunkPos);
}

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

const std::vector<Entity>& Level::GetEntitiesAtPosition(const IntVector& position)
{
    if (Chunk::EntityContainer* container = TryGetEntityContainer(position); container != nullptr)
    {
        return container->Entities;
    }
    return EmptyEntities;
}

void Level::OnEntityLocationChanged(const GameplayMessage& baseMsg)
{
    // This will trigger if something tries to spawn before the map has been loaded
    assert(Chunks.size() > 0);
    
    auto message = dynamic_cast<const LocationChangedMessage*>(&baseMsg);
    if (message == nullptr)
        return;

    if (message->OldLocation == message->NewLocation)
        return;

    if (Chunk::EntityContainer* removeFrom = TryGetEntityContainer(message->OldLocation))
    {
        auto& entities = removeFrom->Entities;
        auto entityIter = std::find(entities.begin(), entities.end(), message->Owner);
        if (entityIter != entities.end())
        {
            entities.erase(entityIter);
        }
    }
    
    Chunk::EntityContainer* addTo = TryGetEntityContainer(message->NewLocation);
    
    // This is okay, chunk is not currently loaded into memory. Ignore.
    if (addTo == nullptr)
        return;

#if _DEBUG
    auto containsIter = std::find(addTo->Entities.begin(), addTo->Entities.end(), message->Owner);
    assert(containsIter == addTo->Entities.end());
#endif
    
    addTo->Entities.push_back(message->Owner);
}

Level::Chunk::EntityContainer* Level::TryGetEntityContainer(const IntVector& worldPosition)
{
    IntVector chunkPosition = WorldPositionUtility::WorldPositionToChunkPosition(worldPosition);
    IntVector chunkSpace = WorldPositionUtility::WorldSpaceToChunkSpace(worldPosition);

    auto chunkEntry = Chunks.find(chunkPosition);
    if (chunkEntry == Chunks.end())
        return nullptr;

    if (!WorldPositionUtility::IsChunkSpacePositionInChunk(chunkSpace))
        return nullptr;

    int32_t idx = WorldPositionUtility::GetTileIndex(chunkSpace);
    return &chunkEntry->second.Tiles[idx];
}
