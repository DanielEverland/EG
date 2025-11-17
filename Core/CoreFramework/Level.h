// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "Navigation.h"
#include "Components/LocationComponent.h"
#include "DataStructrues/Vector.h"
#include "ECS/ComponentManager.h"
#include "Utilities/WorldPositionUtility.h"

class MapChunk;
class Game;

class Level
{
    struct Chunk
    {
        struct EntityContainer
        {
            std::vector<Entity> Entities;
        };

        EntityContainer Tiles[WorldPositionUtility::ChunkWidth * WorldPositionUtility::ChunkHeight];
    };
    
public:
    Level();

    void LoadChunk(const IntVector& chunkPos, std::shared_ptr<MapChunk> chunk);
    bool DoesWorldPositionExist(const IntVector& worldPosition);
    
    Entity CreateEntity() const;
    Entity CreateEntity(const std::string& templateId) const;
    Entity CreateEntity(const std::string& templateId, const IntVector2D& position) const;
    
    void DestroyEntity(Entity entity);
    ComponentManager& GetComponentManager() { return Components; }
    const std::vector<Entity>& GetEntitiesAtPosition(const IntVector& position);
    const Navigation& GetNavigation() const { return NavigationInstance; }
    
private:
    Navigation NavigationInstance;
    ComponentManager Components;
    std::unordered_map<IntVector, Chunk> Chunks;
    std::vector<Entity> EmptyEntities;

    void OnEntityLocationChanged(const GameplayMessage& message);
    Chunk::EntityContainer* TryGetEntityContainer(const IntVector& worldPosition); 
};
