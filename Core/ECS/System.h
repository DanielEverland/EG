// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cstdint>
#include <memory>
#include <utility>

#include "CoreFramework/Game.h"
#include "CoreFramework/Level.h"

class Game;

class System
{
public:
    System() = default;
    virtual ~System() = default;

    // Priority defines the order in which systems are executed
    // Higher priority systems are executed first
    [[nodiscard]] virtual int32_t GetPriority() const { return 0; }
    
    virtual void Execute() = 0;

    std::shared_ptr<Level> GetLevel() const;

    template<class... T, std::invocable<T&...> F>
    void Query(F&& func)
    {
        auto iter = GetComponentManager().CreateIterator<T...>();
        iter.Execute(std::forward<F>(func));
    }

private:
    ComponentManager& GetComponentManager() const { return GetLevel()->GetComponentManager(); }
};
