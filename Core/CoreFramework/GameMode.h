// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "ECS/Entity.h"

class Game;

class GameMode
{
public:
    virtual ~GameMode() = default;

    virtual void Initialize() = 0;

    Entity GetPossessedEntity() const { return PossessedEntity; }

protected:
    void SetPossessedEntity(Entity entity);
    
private:
    Entity PossessedEntity = InvalidEntity;
};
