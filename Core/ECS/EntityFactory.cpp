#include "EntityFactory.h"

#include <fstream>
#include <iostream>

#include "Components/LocationComponent.h"
#include "CoreFramework/AssetManager.h"
#include "CoreFramework/Game.h"
#include "CoreFramework/Level.h"

bool EntityFactory::TemplateParser::TryParse(json data)
{
    auto idVal = data.find("Id");
    if (idVal == data.end())
        return false;
    Id = idVal->get<std::string>();

    auto composites = data.find("Composites");
    if (composites != data.end())
    {
        Composites = composites->get<std::vector<std::string>>();
    }

    auto components = data.find("Components");
    if (components ==  data.end())
        return false;   // TODO: Not sure if this should be a requirement

    for(const auto& component : components.value())
    {
        ComponentFactory::Parameters params;

        auto compIdVal = component.find("Id");
        if (compIdVal == component.end())
            return false;
        params.Id = compIdVal->get<std::string>();
        
        for (auto iter = component.begin(); iter != component.end(); ++iter)
        {
            if (iter.key() == "Id")
                continue;
            params.Properties.emplace(iter.key(), iter.value());
        }

        Components.push_back(params);
    }
    
    return true;
}

void EntityFactory::Load()
{
    std::vector<std::filesystem::path> templatePaths = AssetManager::Get().GetAssets(R"(Entities\)");
    for (std::filesystem::path& path : templatePaths)
    {
        std::ifstream stream(path);
        const json data = json::parse(stream);
        stream.close();
        
        TemplateParser parser;
        if (parser.TryParse(data))
        {
            ParserLookup.emplace(parser.GetId(), std::move(parser));
        }
    }
}

void EntityFactory::PopulateEntity(Entity entity, std::string templateId)
{
    if (!ParserLookup.contains(templateId))
    {
        std::cerr << "Unable to find entity template for " << templateId << std::endl;
        abort();
    }

    const TemplateParser& parser = ParserLookup[templateId];
    for (const std::string& compositeId : parser.GetComposites())
    {
        PopulateEntity(entity, compositeId);
    }

    std::shared_ptr<Level> level = Game::Get().GetLevel();
    ComponentManager& componentManager = level->GetComponentManager();

    for (const ComponentFactory::Parameters& componentParams : parser.GetComponents())
    {
        ComponentFactory::CreateEntity(entity, componentParams, componentManager);
    }
}

void EntityFactory::PopulateEntity(Entity entity, std::string templateId, IntVector2D worldLocation)
{
    PopulateEntity(entity, templateId);

    std::shared_ptr<Level> level = Game::Get().GetLevel();
    ComponentManager& componentManager = level->GetComponentManager();

    if (LocationComponent* comp = componentManager.TryGetComponent<LocationComponent>(entity))
    {
        comp->WorldLocation = worldLocation;
    }
}
