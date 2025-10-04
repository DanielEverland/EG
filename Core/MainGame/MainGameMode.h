// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "CoreFramework/GameMode.h"

class MainGameMode : public GameMode
{
public:
    using GameMode::GameMode;

    void Initialize() override;

private:
    void RegisterInput();
    void HandleMovementInput(int32_t value, bool bIsHorizontal);
    Entity CreateCreature(Entity target = InvalidEntity) const;
};
