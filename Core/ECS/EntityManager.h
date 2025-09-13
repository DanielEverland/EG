// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "Entity.h"

class EntityManager
{
public:

private:
    Entity NextAvailableEntityId = 0;
    std::vector<Entity> ActiveEntities;

    Entity GetNewEntityId() { return NextAvailableEntityId++; }
};
