// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include "ECS/System.h"

class MovementSystem : public System
{
public:
    void Execute() override;
    bool IsValidMove(Entity entity, const IntVector2D& currentPosition, const IntVector2D& targetPosition) const;

protected:
    void RegisterIterators() override;
};
