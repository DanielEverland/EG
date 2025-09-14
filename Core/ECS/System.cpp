#include "System.h"

std::shared_ptr<Game> System::GetGame() const
{
    return Game;    
}
std::shared_ptr<Level> System::GetLevel() const
{
    return Game->GetLevel();
}
