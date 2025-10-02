#include "CreatureAISystem.h"

#include "Components/CreatureComponent.h"
#include "Components/LocationComponent.h"
#include "CoreFramework/GameMode.h"

void CreatureAISystem::Execute()
{
    auto gameMode = Game::Get().GetGameMode();
    Query<LocationComponent, CreatureComponent>(
        [gameMode](const Entity entity, LocationComponent& location, const CreatureComponent& creature)
        {
            const Entity possessedEntity = gameMode->GetPossessedEntity();
            if (possessedEntity == InvalidEntity || possessedEntity == entity)
                return;

            auto level = Game::Get().GetLevel();
            auto targetLocationComp = level->GetComponentManager().TryGetComponent<LocationComponent>(possessedEntity);
            if (!targetLocationComp)
                return;

            IntVector2D direction = targetLocationComp->WorldLocation - location.WorldLocation;
            direction.Normalize();
            
            const IntVector2D newPosition = location.WorldLocation + direction;
            if (newPosition == targetLocationComp->WorldLocation)
                return;

            location.WorldLocation = newPosition;
        });
}
