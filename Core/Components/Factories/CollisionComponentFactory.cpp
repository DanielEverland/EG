#include "CollisionComponentFactory.h"
#include "Components/CollisionComponent.h"

void CollisionComponentFactory::Populate(Entity entity, const Parameters& params, ComponentManager& manager)
{
    manager.AddComponent<CollisionComponent>(entity);
}
