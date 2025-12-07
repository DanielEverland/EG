// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "CoreFramework/DebugRenderer.h"
#include "DataStructrues/Vector.h"
#include "ECS/Component.h"

#include "Debugging/LocationComponentDebugDrawer.h"
#include "Factories/LocationComponentFactory.h"
#include "GameplayMessages/GameplayMessages.h"
REGISTER_COMP_FACTORY(LocationComponentFactory, "LocationComponent");

class LocationComponent : public Component
{
public:
    const IntVector& GetLocation() const { return WorldLocation; }
    void SetLocation(Entity owner, const IntVector& newLocation);

private:
    IntVector WorldLocation = IntVector(INT_MAX, INT_MAX, INT_MAX);
};

REGISTER_DEBUGGER(LocationComponent, LocationComponentDebugDrawer);