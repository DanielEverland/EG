// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "CoreFramework/Game.h"
#include "CoreFramework/Level.h"

template<class... Args>
class SystemOperation
{
public:
    SystemOperation() = default;
    SystemOperation(System* system, std::function<void(Args&...)> f) : storedFunction(f)
    {
        SystemMutexCollection::Get().RegisterComponentsToLock<Args...>(system);
    }

protected:
    std::function<void(Args&...)> storedFunction;
    
    ComponentManager::Iter<Args ...> CreateIterator()
    {
        std::shared_ptr<Level> level = Game::Get().GetLevel();
        ComponentManager& componentManager = level->GetComponentManager();
        return componentManager.CreateIterator<Args...>();
    }
};


template<class... Args>
class QueryOperation : public SystemOperation<Args...>
{
public:
    using SystemOperation<Args...>::SystemOperation;
    
    void Execute()
    {
        ComponentManager::Iter<Args ...> iter = this->CreateIterator();
        iter.Execute(this->storedFunction);
    }
};

template<class... Args>
class AnyOperation : public SystemOperation<Args...>
{
public:
    using SystemOperation<Args...>::SystemOperation;
    
    bool Execute()
    {
        ComponentManager::Iter<Args ...> iter = this->CreateIterator();
        return iter.Any(this->storedFunction);
    }
};