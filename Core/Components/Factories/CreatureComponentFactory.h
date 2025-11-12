// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include "ECS/ComponentFactory.h"

class CreatureComponentFactory : public ComponentFactory
{
public:
    using ComponentFactory::ComponentFactory;

    void Populate(Entity entity, const Parameters& params, ComponentManager& manager) override;
};
