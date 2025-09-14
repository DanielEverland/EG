// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <memory>

class Game;

class GameMode
{
public:
    GameMode(std::shared_ptr<Game> game) : GameInstance(std::move(game)) { }

    std::shared_ptr<Game> GetGame() const { return GameInstance; }
    virtual void Initialize() = 0;
    
private:
    std::shared_ptr<Game> GameInstance;
};
