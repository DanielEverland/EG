#include "AttackSystem.h"

#include "Combat/AttackEntry.h"
#include "Components/AttackerComponent.h"
#include "Components/DeathComponent.h"
#include "Components/HealthComponent.h"

void AttackSystem::Execute()
{
    ComponentManager& compManager = Game::Get().GetLevel()->GetComponentManager();
    Query<AttackerComponent>(
        [this, &compManager](AttackerComponent& attackerComponent)
        {
            if (attackerComponent.PendingAttack == nullptr)
                return;

            std::shared_ptr<AttackEntry> pendingAttack = attackerComponent.PendingAttack;
            attackerComponent.PendingAttack = nullptr;

            pendingAttack->Execute(compManager);

            for (Entity victim : pendingAttack->GetTargetEntities())
            {
                if (auto healthComponent = compManager.TryGetComponent<HealthComponent>(victim))
                {
                    if (healthComponent->Health <= FLT_EPSILON)
                    {
                        compManager.AddComponent<DeathComponent>(victim);
                    }
                }
            }
        });
}
