// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cstdint>

class System
{
public:
    virtual ~System() = default;

    // Priority defines the order in which systems are executed
    // Higher priority systems are executed first
    [[nodiscard]] virtual int32_t GetPriority() const { return 0; }
    
    virtual void Execute() = 0;
};
