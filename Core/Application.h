#pragma once

#include "CoreFramework/Game.h"

class Application
{
public:
    void Initialize();
    void Tick();

private:
    std::shared_ptr<Game> GameInstance;
};
