// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "ComponentManager.h"
#include "Entity.h"
#include "json.hpp"

class ComponentManager;

#define LOAD_PROPERTY(Comp, MemberName) TryLoadProperty(params, Comp.MemberName, #MemberName);

class ComponentFactory
{
public:
    struct Parameters
    {
        std::string Id;
        std::unordered_map<std::string, nlohmann::json> Properties;
    };
    
    virtual ~ComponentFactory() = default;
    ComponentFactory() = default;
    
    virtual void Populate(Entity entity, const Parameters& params, ComponentManager& manager) = 0;

    static void Register(const std::string& componentId, std::unique_ptr<ComponentFactory>&& factory);
    static void CreateEntity(Entity entity, const Parameters& params, ComponentManager& manager);

protected:
    template<class T>
    bool TryLoadProperty(const Parameters& params, T& targetVar, const std::string& propertyName)
    {
        const auto& prop = params.Properties.find(propertyName);
        if (prop != params.Properties.end())
        {
            targetVar = prop->second.get<T>();
            return true;
        }
        return false;
    }

private:
    static std::unordered_map<std::string, std::unique_ptr<ComponentFactory>>* ComponentFactoryLookup;
};

template <class T>
struct ComponentFactoryRegistrar {
    ComponentFactoryRegistrar(const std::string& componentId)
    {
        std::unique_ptr<T> ptr = std::make_unique<T>();
        ComponentFactory::Register(componentId, std::move(ptr));
    }
};

#define REGISTER_COMP_FACTORY(TYPE, NAME) \
    inline ComponentFactoryRegistrar<TYPE> _AUTOGEN_##TYPE { NAME };
