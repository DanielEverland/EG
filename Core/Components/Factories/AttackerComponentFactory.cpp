#include "AttackerComponentFactory.h"

#include "Components/AttackerComponent.h"

void AttackerComponentFactory::Populate(Entity entity, const Parameters& params, ComponentManager& manager)
{
    manager.AddComponent<AttackerComponent>(entity);
}
