// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "CoreFramework/GameMode.h"
#include "CoreFramework/Level.h"

class MainGameMode : public GameMode
{
public:
    using GameMode::GameMode;

    void CreatePlayer(std::shared_ptr<Level> level, ComponentManager& componentManager);
    void LoadMap(std::shared_ptr<Level> level);
    void SpawnEnemies(std::shared_ptr<Level> level, ComponentManager& componentManager);
    void Initialize() override;

private:
    void RegisterInput();
    void HandleMovementInput(int32_t value, bool bIsHorizontal);
    void HandleCameraZoom(int32_t value);
};
