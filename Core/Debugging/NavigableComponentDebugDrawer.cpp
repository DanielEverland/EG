#include "NavigableComponentDebugDrawer.h"

#include "imgui.h"
#include "Components/NavigableComponent.h"

void NavigableComponentDebugDrawer::Draw(Component *comp)
{
    auto nc = dynamic_cast<NavigableComponent*>(comp);
    ImGui::Text("Climbable: %s", nc->Climbable ? "true" : "false");
    ImGui::Text("Traversal Difficulty: %d", nc->TraversalDifficulty);
}
