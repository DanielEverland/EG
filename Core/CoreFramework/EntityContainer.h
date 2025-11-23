// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <vector>

#include "ECS/Entity.h"
#include "ECS/ComponentManager.h"

class EntityContainer
{
public:    
    std::vector<Entity> Entities;

    template<class T>
    T* TryGetFirstComponent() const
    {
        for (Entity entity : Entities)
        {
            if (auto comp = GetComponentManager().TryGetComponent<T>(entity))
            {
                return comp;
            }
        }
        return nullptr;
    }

    template<class T, typename Predicate>
    T* TryGetFirstComponent(Predicate pred) const
    {
        for (Entity entity : Entities)
        {
            if (auto comp = GetComponentManager().TryGetComponent<T>(entity))
            {
                if (pred(comp))
                {
                    return comp;
                }
            }
        }
        return nullptr;
    }
    
    template<class T>
    bool AnyHasComponent() const
    {
        for (Entity entity : Entities)
        {
            if (GetComponentManager().ContainsComponent<T>(entity))
                return true;
        }
        return false;
    }

    template<class T, typename Predicate>
    bool AnyHasComponent(Predicate pred) const
    {
        for (Entity entity : Entities)
        {
            if (GetComponentManager().ContainsComponent<T>(entity, pred))
                return true;
        }
        return false;
    }

    template<class T>
    bool AllHaveComponent() const
    {
        for (Entity entity : Entities)
        {
            if (!GetComponentManager().ContainsComponent<T>(entity))
                return false;
        }
        return true;
    }

private:
    ComponentManager& GetComponentManager() const;
};
