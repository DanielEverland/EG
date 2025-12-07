#include "LocationComponentFactory.h"
#include "Components/LocationComponent.h"

void LocationComponentFactory::Populate(Entity entity, const Parameters& params, ComponentManager& manager)
{
    LocationComponent& comp = manager.AddComponent<LocationComponent>(entity);
    comp.SetLocation(entity, IntVector(INT_MAX, INT_MAX, INT_MAX));
}
