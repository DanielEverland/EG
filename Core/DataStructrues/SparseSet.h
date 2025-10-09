// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <unordered_map>

#include "ECS/Entity.h"

// Implemented using: https://programmingpraxis.com/2012/03/09/sparse-sets/
// But with satellite data
template<class T>
class SparseSet
{
    using ValueType = std::remove_cv_t<T>;
    
public:
    struct Entry
    {
        Entry(const Entity entity, ValueType value) : EntityId(entity), Data(value) {}

        Entry& operator=(const Entry& other) = default;
        
        Entity EntityId;
        ValueType Data;
    };
    
    ValueType& AddDefaulted(Entity entity)
    {
        if (Contains(entity))
            return Get(entity);
        
        Dense.push_back(Entry(entity, T() ));
        Sparse[entity] = Dense.size() - 1;
        return Dense.back().Data;
    }

    ValueType& Get(const Entity entity)
    {
        return Dense[Sparse.at(entity)].Data;
    }

    const ValueType& Get(const Entity entity) const
    {
        return Dense[Sparse.at(entity)].Data;
    }

    void Remove(const Entity entity)
    {
        // Swap out the back entry with the entry to remove
        const auto indexIter = Sparse.find(entity);
        if (indexIter == Sparse.end())
            return;

        size_t index = indexIter->second;
        Dense[index] = Dense.back();
        Sparse[Dense[index].EntityId] = index;
        Dense.pop_back();
        Sparse.erase(entity);
    }

    [[nodiscard]] bool Contains(const Entity entity) const
    {
        return Sparse.contains(entity);
    }

    [[nodiscard]] size_t Size() const
    {
        return Dense.size();
    }

    std::vector<Entry>::iterator begin() { return Dense.begin(); }
    std::vector<Entry>::iterator end() { return Dense.end(); }

private:
    std::vector<Entry> Dense;
    std::unordered_map<Entity, size_t> Sparse;
};
