// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <ostream>

#include "ComponentContainer.h"
#include "Entity.h"

class ComponentManager
{
public:
    template<class... T>
    requires (std::is_base_of_v<Component, T> && ...)
    class Iter
    {
    public:
        Iter(ComponentManager& manager)
            :   Manager(manager),
                Containers { Manager.GetContainer<T>()... }
        {
        }
        // Move constructor is dangerous due to manager field being a reference
        Iter(ComponentManager&& manager) = delete;

        template<std::invocable<T&...> F>
        void Execute(F&& func)
        {
            // TODO: Optimize by choosing the smallest container as driver
            auto& driver = std::get<0>(Containers);            
            auto fn = std::forward<F>(func);
            for (auto& element : driver) {
                if ((std::get<ComponentContainer<T>&>(Containers).Contains(element.EntityId) && ...)) {
                    fn(std::get<ComponentContainer<T>&>(Containers).Get(element.EntityId)...);
                }
            }
        }
        
    private:
        ComponentManager& Manager;
        std::tuple<ComponentContainer<T>&...> Containers;
    };

    template<class T>
    void AddComponent(Entity entity);

    template<class... T>
    Iter<T...> CreateIterator()
    {
        return Iter<T...>(*this);
    }

private:
    std::unordered_map<size_t, std::unique_ptr<IComponentContainer>> ComponentContainers;
    
    template<class T>
    ComponentContainer<T>& GetContainer();
};

template <class T>
void ComponentManager::AddComponent(Entity entity)
{
    ComponentContainer<T>& Container = GetContainer<T>();
    Container.Add(entity);
}

template <class T>
ComponentContainer<T>& ComponentManager::GetContainer()
{
    const size_t typeHash = typeid(T).hash_code();

    if (!ComponentContainers.contains(typeHash))
    {
        auto newContainer  = std::make_unique<ComponentContainer<T>>();
        ComponentContainers[typeHash] = std::move(newContainer);
    }

    IComponentContainer* component = ComponentContainers[typeHash].get();
    auto casted = dynamic_cast<ComponentContainer<T>*>(component);
    
    assert(casted != nullptr);
    
    return *casted;
}
