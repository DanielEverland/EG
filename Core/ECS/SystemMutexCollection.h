// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <typeinfo>
#include <unordered_map>
#include <unordered_set>

#include "CoreFramework/Level.h"

class System;

class SystemMutexCollection
{    
public:
    static SystemMutexCollection& Get();

    template<class T, class... Rest>
    void RegisterComponentsToLock(System* system)
    {
        RegisterComponent<T>(system);

        if constexpr (sizeof...(Rest))
        {
            RegisterComponentsToLock<Rest...>(system);
        }
    }

    void Lock(const System* system);
    void Unlock(const System* system);

private:
    std::unordered_map<const System*, std::unordered_set<size_t>> ComponentsToLock;

    template<class T>
    void RegisterComponent(System* system)
    {
        if (!ComponentsToLock.contains(system))
        {
            ComponentsToLock.emplace(system, std::unordered_set<size_t>());
        }

        ComponentsToLock[system].insert(typeid(T).hash_code());
    }
};
