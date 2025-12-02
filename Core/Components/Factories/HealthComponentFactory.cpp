#include "HealthComponentFactory.h"

#include "Components/HealthComponent.h"

void HealthComponentFactory::Populate(Entity entity, const Parameters& params, ComponentManager& manager)
{
    HealthComponent& healthComponent = manager.AddComponent<HealthComponent>(entity);

    LOAD_PROPERTY(healthComponent, Health);
}
