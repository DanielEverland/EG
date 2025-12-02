#include "MeleeAttackEntry.h"

#include "Components/HealthComponent.h"
#include "ECS/ComponentManager.h"

void MeleeAttackEntry::Execute(ComponentManager& compManager)
{
    if (HealthComponent* healthComponent = compManager.TryGetComponent<HealthComponent>(Target))
    {
        healthComponent->Health -= 15.0f;
    }
}

std::vector<Entity> MeleeAttackEntry::GetTargetEntities() const
{
    return { Target };
}
