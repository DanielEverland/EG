// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "ECS/System.h"

class RenderingSystem : public System
{
public:
    using System::System;
    
    void Execute() override;
};
