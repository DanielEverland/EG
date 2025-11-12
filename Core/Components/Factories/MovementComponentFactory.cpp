#include "MovementComponentFactory.h"

#include "Components/MovementComponent.h"

void MovementComponentFactory::Populate(Entity entity, const Parameters& params, ComponentManager& manager)
{
    auto& movementComp = manager.AddComponent<MovementComponent>(entity);

    auto movementSpeed = params.Properties.find("MovementSpeed");
    if (movementSpeed != params.Properties.end())
    {
        movementComp.MovementSpeed = movementSpeed->second.get<float>();
    }
}
