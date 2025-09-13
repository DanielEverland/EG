#include "GameComponent.h"

#include <cassert>

GameComponent::GameComponent(Game* game) : GameReference(game)
{
    assert(game != nullptr);
}
