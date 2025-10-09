// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <shared_mutex>
#include <unordered_map>

#include "Entity.h"
#include "DataStructrues/SparseSet.h"

class Component;

class IComponentContainer
{
public:
    virtual ~IComponentContainer() = default;

    virtual void LockWritable() = 0;
    virtual void UnlockWritable() = 0;
    virtual void Add(Entity entity) = 0;
    virtual bool Contains(Entity entity) = 0;
    virtual void RemoveEntity(Entity entity) = 0;
    [[nodiscard]] virtual size_t Size() const = 0;
};

// Contains the component of a single type of component
// Looks up using a sparse set https://programmingpraxis.com/2012/03/09/sparse-sets/
template<class T>
requires std::is_base_of_v<Component, T>
class ComponentContainer : public IComponentContainer
{    
public:
    std::shared_mutex Mutex;
    
    ComponentContainer() = default;

    // We shouldn't ever copy containers
    ComponentContainer(const ComponentContainer& other) = delete;

    void LockWritable() override
    {
        Mutex.lock();
    }

    void UnlockWritable() override
    {
        Mutex.unlock();
    }
    
    void Add(Entity entity) override
    {
        Components.AddDefaulted(entity);
    }

    bool Contains(Entity entity) override
    {
        return Components.Contains(entity);
    }

    void RemoveEntity(Entity entity) override
    {
        Components.Remove(entity);
    }

    T& Get(Entity entity)
    {
        return Components.Get(entity);
    }

    [[nodiscard]] size_t Size() const override
    {
        return Components.Size();
    }

    std::vector<typename SparseSet<T>::Entry>::iterator begin()
    {
        return Components.begin();
    }

    std::vector<typename SparseSet<T>::Entry>::iterator end()
    {
        return Components.end();
    }

private:
    SparseSet<T> Components;
};
