// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "ECS/ComponentFactory.h"

class CollisionComponentFactory : public ComponentFactory
{
public:
    void Populate(Entity entity, const Parameters& params, ComponentManager& manager) override;
};
