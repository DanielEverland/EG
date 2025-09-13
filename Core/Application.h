#pragma once
#include "ECS/ComponentManager.h"

class Application
{
public:
    void Initialize();
    void Tick();

private:
    ComponentManager ComponentManager;
};
