#include "System.h"

std::shared_ptr<Level> System::GetLevel() const
{
    return Game::Get().GetLevel();
}
