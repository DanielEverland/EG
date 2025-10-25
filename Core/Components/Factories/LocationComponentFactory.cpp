#include "LocationComponentFactory.h"
#include "Components/LocationComponent.h"

void LocationComponentFactory::Populate(Entity entity, const Parameters& params, ComponentManager& manager)
{
    manager.AddComponent<LocationComponent>(entity);
}
