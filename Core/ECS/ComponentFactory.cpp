#include "ComponentFactory.h"

std::unordered_map<std::string, std::unique_ptr<ComponentFactory>>* ComponentFactory::ComponentFactoryLookup = nullptr;

void ComponentFactory::Register(const std::string& componentId, std::unique_ptr<ComponentFactory>&& factory)
{
    if (ComponentFactoryLookup == nullptr)
    {
        ComponentFactoryLookup = new std::unordered_map<std::string, std::unique_ptr<ComponentFactory>>();
    }
    
    assert(!ComponentFactoryLookup->contains(componentId));
    ComponentFactoryLookup->emplace(componentId, std::move(factory));
}

void ComponentFactory::CreateEntity(Entity entity, const Parameters& params, ComponentManager& manager)
{
    if (ComponentFactoryLookup == nullptr)
        return;

    auto entry = ComponentFactoryLookup->find(params.Id);
    if (entry == ComponentFactoryLookup->end())
        return;

    entry->second->Populate(entity, params, manager);
}
