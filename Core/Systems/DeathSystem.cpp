#include "DeathSystem.h"

#include "Components/DeathComponent.h"

void DeathSystem::Execute()
{
    std::shared_ptr<Level> Level = Game::Get().GetLevel();
    Query<DeathComponent>(
        [this, Level](const Entity entity, const DeathComponent& _)
        {
            Level->DestroyEntity(entity);
        });
}
