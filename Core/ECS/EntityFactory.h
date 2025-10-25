// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "ComponentFactory.h"
#include "Entity.h"
#include "json.hpp"
#include "DataStructrues/Vector.h"

using namespace nlohmann;

class EntityFactory
{
    class TemplateParser
    {
    public:
        const std::string& GetId() const { return Id; }
        const std::vector<ComponentFactory::Parameters>& GetComponents() const { return Components; }
        const std::vector<std::string>& GetComposites() const { return Composites; }
        
        bool TryParse(json data);

    private:
        std::string Id;
        std::vector<ComponentFactory::Parameters> Components;
        std::vector<std::string> Composites;
    };
    
public:
    void Load();
    void PopulateEntity(Entity entity, std::string templateId);
    void PopulateEntity(Entity entity, std::string templateId, IntVector2D worldLocation);

private:
    std::unordered_map<std::string, TemplateParser> ParserLookup;
};
