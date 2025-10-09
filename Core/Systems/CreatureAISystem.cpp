#include "CreatureAISystem.h"

#include "Components/CreatureComponent.h"
#include "Components/LocationComponent.h"
#include "Components/MovementComponent.h"
#include "CoreFramework/GameMode.h"

void CreatureAISystem::Execute()
{
    auto gameMode = Game::Get().GetGameMode();
    /*Query<LocationComponent, CreatureComponent, MovementComponent>(
        [this, gameMode](const Entity entity, LocationComponent& location, const CreatureComponent& creature, MovementComponent& movementComponent)
        {
            const Entity possessedEntity = gameMode->GetPossessedEntity();
            if (possessedEntity == InvalidEntity || possessedEntity == entity)
                return;

            auto level = Game::Get().GetLevel();
            auto targetLocationComp = level->GetComponentManager().TryGetComponent<LocationComponent>(possessedEntity);
            if (!targetLocationComp)
                return;

            movementComponent.TargetLocation = targetLocationComp->WorldLocation;
        })*/;
}
