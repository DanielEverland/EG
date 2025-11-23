// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "EntityContainer.h"
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
        EntityContainer Tiles[WorldPositionUtility::ChunkWidth * WorldPositionUtility::ChunkHeight];
    };
    
public:
    Level();

    void LoadChunk(const IntVector& chunkPos, std::shared_ptr<MapChunk> chunk);
    bool DoesWorldPositionExist(const IntVector& worldPosition);
    
    Entity CreateEntity() const;
    Entity CreateEntity(const std::string& templateId) const;
    Entity CreateEntity(const std::string& templateId, const IntVector& position) const;

    template<class T>
    bool EntityHasComponent(Entity entity)
    {
        return GetComponentManager().ContainsComponent<T>(entity);
    }

    template<class T, typename Predicate>
    bool EntityHasComponent(Entity entity, Predicate pred)
    {
        if (T* comp = TryGetComponent<T>(entity))
        {
            return pred(comp);
        }
        return false;
    }

    template<class T>
    T* TryGetComponent(Entity entity)
    {
        return GetComponentManager().TryGetComponent<T>(entity);
    }
    
    void DestroyEntity(Entity entity);
    ComponentManager& GetComponentManager() { return Components; }
    const EntityContainer& GetEntitiesAtPosition(const IntVector& position);
    bool IsValidMove(Entity entity, const IntVector& from, const IntVector& to) const;
    const Navigation& GetNavigation() const { return NavigationInstance; }
    
private:
    EntityContainer EmptyEntityContainer;
    Navigation NavigationInstance;
    ComponentManager Components;
    std::unordered_map<IntVector, Chunk> Chunks;
    std::vector<Entity> EmptyEntities;

    void OnEntityLocationChanged(const GameplayMessage& message);
    EntityContainer* TryGetEntityContainer(const IntVector& worldPosition); 
};
