#include "CreatureAISystem.h"

#include "Components/CreatureComponent.h"
#include "Components/LocationComponent.h"
#include "Components/MovementComponent.h"
#include "CoreFramework/GameMode.h"
#include "CoreFramework/LevelNavigationGraphTraverser.h"
#include "CoreFramework/NavRequest.h"

void CreatureAISystem::Execute()
{
    auto gameMode = Game::Get().GetGameMode();
    Query<LocationComponent, CreatureComponent, MovementComponent>(
        [this, gameMode](const Entity entity, LocationComponent& location, const CreatureComponent& creature, MovementComponent& movementComponent)
        {
            const Entity possessedEntity = gameMode->GetPossessedEntity();
            if (possessedEntity == InvalidEntity || possessedEntity == entity)
                return;

            auto level = Game::Get().GetLevel();
            auto targetLocationComp = level->GetComponentManager().TryGetComponent<LocationComponent>(possessedEntity);
            if (!targetLocationComp)
                return;

            const Navigation& nav = level->GetNavigation();

            NavRequest request;
            request.EntityToMove = entity;
            request.StartPosition = location.GetLocation();
            request.TargetPosition = targetLocationComp->GetLocation();
            request.Traverser = std::make_shared<LevelNavigationGraphTraverser>(level);
            NavResult result = nav.TryCalculatePath(request);

            if (result.Succeeded)
            {
                // Always includes start, so we pop that off.
                result.Path.pop_front();
                movementComponent.TargetLocation = result.Path.front();
            }
            else
            {
                movementComponent.TargetLocation = targetLocationComp->GetLocation();
            }
        });
}
