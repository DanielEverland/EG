// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "GameplayMessages.h"
#include "DataStructrues/Vector.h"

struct LocationChangedMessage : GameplayMessage
{
    Entity Owner;
    IntVector OldLocation;
    IntVector NewLocation;
};;
