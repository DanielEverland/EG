#include "NavigableComponentFactory.h"

#include "Components/NavigableComponent.h"

void NavigableComponentFactory::Populate(Entity entity, const Parameters& params, ComponentManager& manager)
{
    auto& navigableComponent = manager.AddComponent<NavigableComponent>(entity);
    
    LOAD_PROPERTY(navigableComponent, TraversalDifficulty)
    assert(navigableComponent.TraversalDifficulty != 0);
}
