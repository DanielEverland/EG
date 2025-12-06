// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include "DebugDrawer.h"

class HealthComponentDebugDrawer : public DebugDrawer
{
public:
    void Draw(Component *comp) override;
};
