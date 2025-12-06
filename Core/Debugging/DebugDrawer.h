// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

class DebugDrawer
{
public:
    virtual ~DebugDrawer() = default;
    virtual void Draw(class Component* comp) { }
};
