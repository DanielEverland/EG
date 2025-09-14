// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "CoreFramework/GameMode.h"

class MainGameMode : public GameMode
{
public:
    using GameMode::GameMode;

    void Initialize() override;
};
