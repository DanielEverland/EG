#include "TextureRendererComponentDebugDrawer.h"

#include "imgui.h"
#include "Components/TextureRendererComponent.h"

void TextureRendererComponentDebugDrawer::Draw(Component *comp)
{
    auto trc = dynamic_cast<TextureRendererComponent*>(comp);
    ImGui::Text("Order: %d", trc->Order);
    ImGui::Text("Texture: %s", trc->TextureName.ToString().c_str());
}
