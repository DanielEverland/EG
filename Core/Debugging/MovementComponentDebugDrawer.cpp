#include "MovementComponentDebugDrawer.h"

#include "imgui.h"
#include "Components/MovementComponent.h"
void MovementComponentDebugDrawer::Draw(Component *comp)
{
    auto msc = dynamic_cast<MovementComponent*>(comp);
    ImGui::Text("Speed: %.2f", msc->MovementSpeed);
    ImGui::SameLine();
    ImGui::Text("Remainder: %.2f", msc->MovementSpeedRemainder);
    ImGui::Text("Target Location: %s", msc->TargetLocation.ToString().c_str());
}
