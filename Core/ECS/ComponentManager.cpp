#include "ComponentManager.h"

std::vector<Component*> ComponentManager::GetAllComponents(Entity entity)
{
    std::vector<Component*> allComponents;
    for (const auto& element : ComponentContainers)
    {
        IComponentContainer* container = element.second.get();
        if (container->Contains(entity))
        {
            allComponents.push_back(container->GetBaseComponent(entity));
        }
    }
    return allComponents;
}
