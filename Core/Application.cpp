#include "Application.h"

void Application::Initialize()
{
    GameInstance = std::make_shared<Game>();
    GameInstance->Initialize();
}

void Application::Tick()
{
    GameInstance->Tick();    
}
