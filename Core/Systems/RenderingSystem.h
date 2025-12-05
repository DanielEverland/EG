// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "ECS/System.h"

class RenderingSystem : public System
{
public:
    using System::System;
    
    void Execute() override;

private:
    Rect lastRenderedWorldRect = Rect(0, 0, 0, 0);
    int32_t lastZDepth = 0;

    void DrawEntitiesInCell(const IntVector& worldPosition);
};
