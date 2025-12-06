// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cstdint>

#include "CoreFramework/DebugRenderer.h"
#include "Debugging/NavigableComponentDebugDrawer.h"
#include "ECS/Component.h"

#include "Factories/NavigableComponentFactory.h"
REGISTER_COMP_FACTORY(NavigableComponentFactory, "NavigableComponent")
class NavigableComponent : public Component
{
public:
    uint8_t TraversalDifficulty = 0;
    bool Climbable = false;
};

REGISTER_DEBUGGER(NavigableComponent, NavigableComponentDebugDrawer)