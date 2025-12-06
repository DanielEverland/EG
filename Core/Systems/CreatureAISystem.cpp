#include "CreatureAISystem.h"

#include <iostream>

#include "Combat/MeleeAttackEntry.h"
#include "Components/AttackerComponent.h"
#include "Components/CreatureComponent.h"
#include "Components/LocationComponent.h"
#include "Components/MovementComponent.h"
#include "CoreFramework/GameMode.h"
#include "CoreFramework/LevelNavigationGraphTraverser.h"
#include "CoreFramework/NavRequest.h"

void CreatureAISystem::Execute()
{
    auto gameMode = Game::Get().GetGameMode();
    Query<LocationComponent, CreatureComponent, MovementComponent, AttackerComponent>(
        [this, gameMode](const Entity entity, LocationComponent& location, const CreatureComponent& creature, MovementComponent& movementComponent, AttackerComponent& attacker)
        {
            const Entity possessedEntity = gameMode->GetPossessedEntity();
            if (possessedEntity == InvalidEntity || possessedEntity == entity)
                return;

            auto level = Game::Get().GetLevel();
            auto targetLocationComp = level->GetComponentManager().TryGetComponent<LocationComponent>(possessedEntity);
            if (!targetLocationComp)
                return;

            IntVector diff = targetLocationComp->GetLocation() - location.GetLocation();
            if (diff.Z == 0 && std::max(abs(diff.X), abs(diff.Y)) == 1)
            {
                /*auto attackEntry = std::make_shared<MeleeAttackEntry>();
                attackEntry->Target = possessedEntity;

                attacker.PendingAttack = attackEntry;*/
            }
            else
            {
                const Navigation& nav = level->GetNavigation();

                NavRequest request;
                request.EntityToMove = entity;
                request.StartPosition = location.GetLocation();
                request.TargetPosition = targetLocationComp->GetLocation();
                request.Traverser = std::make_shared<LevelNavigationGraphTraverser>(level);
                request.PermittedTargetDistances = IntVector(1, 1, 0);
                
                NavResult result = nav.TryCalculatePath(request);

                if (result.Succeeded)
                {
                    if (result.Path.size() == 1) // We're already at the target, path only contains our location
                    {
                        movementComponent.TargetLocation = targetLocationComp->GetLocation();
                    }
                    else
                    {
                        // Always includes start, so we pop that off.
                        result.Path.pop_front();
                        movementComponent.TargetLocation = result.Path.front();
                    }
                }
                else
                {
                    movementComponent.TargetLocation = location.GetLocation();
                }
            }
        });
}
