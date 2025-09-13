// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

class Game;

class GameComponent
{
    GameComponent(Game* game);
    
public:
    [[nodiscard]] Game& GetGame() const { return *GameReference; }

private:
    Game* GameReference;
};
