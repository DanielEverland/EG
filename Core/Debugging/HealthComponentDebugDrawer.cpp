#include "HealthComponentDebugDrawer.h"

#include "imgui.h"
#include "Components/HealthComponent.h"

void HealthComponentDebugDrawer::Draw(Component *comp)
{
    auto* hc = static_cast<HealthComponent*>(comp);
    ImGui::Text("Health: %.2f", hc->Health);
}
