// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "ECS/ComponentFactory.h"

class TextureRendererFactory : public ComponentFactory
{
public:
    using ComponentFactory::ComponentFactory;
    
    void Populate(Entity entity, const Parameters& params, ComponentManager& manager) override;
};