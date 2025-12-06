// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include "DebugDrawer.h"

class TextureRendererComponentDebugDrawer : public DebugDrawer
{
public:
    void Draw(Component *comp) override;
};
