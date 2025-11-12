#include "CreatureComponentFactory.h"
#include "Components/CreatureComponent.h"

void CreatureComponentFactory::Populate(Entity entity, const Parameters& params, ComponentManager& manager)
{
    manager.AddComponent<CreatureComponent>(entity);
}
