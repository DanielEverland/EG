// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include "Utilities/SingletonHelpers.h"

class GameSettings
{
public:
    static GameSettings& Get()
    {
        return SingletonHelper::Impl<GameSettings>();
    }

    bool DebugDrawRenderedCells = false;
};
