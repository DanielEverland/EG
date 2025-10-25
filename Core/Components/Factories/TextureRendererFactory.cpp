#include "TextureRendererFactory.h"
#include "Components/TextureRendererComponent.h"
#include "CoreFramework/Game.h"
#include "ECS/ComponentManager.h"

void TextureRendererFactory::Populate(Entity entity, const Parameters& params, ComponentManager& manager)
{
    auto& renderer = manager.AddComponent<TextureRendererComponent>(entity);

    auto textureProp = params.Properties.find("TextureName");
    if (textureProp != params.Properties.end())
    {
        renderer.TextureName = HashedString(textureProp->second);
    }

    auto orderProp = params.Properties.find("Order");
    if (orderProp != params.Properties.end())
    {
        const std::string& order = orderProp->second;
        if (order.compare("Background") == 0)
        {
            renderer.Order = DrawCallOrder::Background;
        }
        else if (order.compare("Foreground") == 0)
        {
            renderer.Order = DrawCallOrder::Foreground;
        }
    }
}
