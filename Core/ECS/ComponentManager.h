// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cassert>
#include <memory>
#include <ranges>
#include <type_traits>

#include "ComponentContainer.h"
#include "Entity.h"

class ComponentManager
{
private:
    template<class... ComponentTypes>
    class IterBase
    {
    public:
        IterBase(ComponentManager& manager)
            :   Manager(manager),
                Containers { Manager.GetContainer<ComponentTypes>()... }
        {
        }
        // Move constructor is dangerous due to manager field being a reference
        IterBase(ComponentManager&& manager) = delete;
        
        template<std::invocable<Entity, ComponentTypes&...> F>
        void EvaluateComponents(F&& func)
        {
            // TODO: Optimize by choosing the smallest container as driver
            auto& driver = std::get<0>(Containers);
            auto fn = std::forward<F>(func);
            for (auto& element : driver) {
                if ((std::get<ComponentContainer<ComponentTypes>&>(Containers).Contains(element.EntityId) && ...)) {
                    fn(element.EntityId, std::get<ComponentContainer<ComponentTypes>&>(Containers).Get(element.EntityId)...);
                }
            }
        }

        template<std::predicate<ComponentTypes&...> F>
        bool Any(F&& func)
        {
            // TODO: Optimize by choosing the smallest container as driver
            auto& driver = std::get<0>(Containers);
            auto fn = std::forward<F>(func);
            for (auto& element : driver) {
                if ((std::get<ComponentContainer<ComponentTypes>&>(Containers).Contains(element.EntityId) && ...)) {
                    if (fn(std::get<ComponentContainer<ComponentTypes>&>(Containers).Get(element.EntityId)...))
                    {
                        return true;
                    }
                }
            }
            return false;
        }
        
    private:
        ComponentManager& Manager;
        std::tuple<ComponentContainer<ComponentTypes>&...> Containers;
    };
    
public:
    template<class... T>
    class Iter : public IterBase<T...>
    {
    public:
        using IterBase<T...>::IterBase;
                
        template<std::invocable<T&...> F>
        void Execute(F&& func)
        {
            auto fn = std::forward<F>(func);
            IterBase<T...>::EvaluateComponents([&fn](Entity entityId, T&... components)
            {
                fn(components...);
            });
        }

        template<std::predicate<T&...> F>
        bool Any(F&& func)
        {
            auto fn = std::forward<F>(func);
            return IterBase<T...>::Any(fn);
        }
    };

    template<class T>
    bool ContainsComponent(Entity entity)
    {
        static_assert(std::derived_from<T, Component> == true, "T does not derive from Component");
        ComponentContainer<T>& Container = GetContainer<T>();
        return Container.Contains(entity);
    }

    template<class T, typename Predicate>
    bool ContainsComponent(Entity entity, Predicate pred)
    {
        static_assert(std::derived_from<T, Component> == true, "T does not derive from Component");
        if (auto comp = TryGetComponent<T>(entity))
        {
            return pred(comp);
        }
        return false;
    }

    template<class T>
    T& AddComponent(Entity entity)
    {
        static_assert(std::derived_from<T, Component> == true, "T does not derive from Component");
        
        ComponentContainer<T>& Container = GetContainer<T>();
        Container.Add(entity);
        return Container.Get(entity);
    }

    void RemoveEntity(Entity entity)
    {
        for (const std::unique_ptr<IComponentContainer>& container : ComponentContainers | std::views::values)
        {
            container->RemoveEntity(entity);
        }
    }

    template<class T>
    T* TryGetComponent(Entity entity)
    {
        static_assert(std::derived_from<T, Component> == true, "T does not derive from Component");
        ComponentContainer<T>& Container = GetContainer<T>();
        if (!Container.Contains(entity))
            return nullptr;
        return &Container.Get(entity);
    }

    template<class T>
    T& GetComponentChecked(Entity entity)
    {
        static_assert(std::derived_from<T, Component> == true, "T does not derive from Component");
        ComponentContainer<T>& Container = GetContainer<T>();
        assert(Container.Contains(entity));
        return Container.Get(entity);
    }

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

// Specialization for Entity + Components iterations
template<class... T>
class ComponentManager::Iter<Entity, T...> : public IterBase<T...>
{
public:
    using IterBase<T...>::IterBase;
    
    template<std::invocable<Entity, T&...> F>
    void Execute(F&& func)
    {
        auto fn = std::forward<F>(func);
        IterBase<T...>::EvaluateComponents([&fn](Entity entityId, T&... components)
        {
            fn(entityId, components...);
        });
    }
};