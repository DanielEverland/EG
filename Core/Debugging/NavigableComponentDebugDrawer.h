// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include "DebugDrawer.h"

class NavigableComponentDebugDrawer : public DebugDrawer
{
public:
    void Draw(Component *comp) override;
};
