// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "DataStructrues/Vector.h"
#include "ECS/Component.h"

#include "Factories/MovementComponentFactory.h"
REGISTER_COMP_FACTORY(MovementComponentFactory, "MovementComponent")

class MovementComponent : public Component
{
public:
    IntVector2D TargetLocation;

    // Movement speed supports fractions. An entity will cache leftover movement used, which can be used later on
    // to move more than one tile per game tick. I.e. with a movement speed of 1.5, an entity will move two tiles
    // every other game tick.
    float MovementSpeed = 1.0f;

    // Leftover movement after a game tick
    float MovementSpeedRemainder = 0.0f;
};
