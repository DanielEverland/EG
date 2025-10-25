// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "ComponentManager.h"
#include "Entity.h"
#include "json.hpp"

class ComponentManager;

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
