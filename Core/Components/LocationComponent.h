// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "DataStructrues/Vector.h"
#include "ECS/Component.h"

#include "Factories/LocationComponentFactory.h"
#include "GameplayMessages/GameplayMessages.h"
REGISTER_COMP_FACTORY(LocationComponentFactory, "LocationComponent");

struct LocationChangedMessage : public GameplayMessage
{
    Entity Owner;
    IntVector OldLocation;
    IntVector NewLocation;
};

class LocationComponent : public Component
{
public:
    const IntVector2D& GetLocation() const { return WorldLocation; }
    void SetLocation(Entity owner, const IntVector2D& newLocation);

private:
    IntVector2D WorldLocation;
};
