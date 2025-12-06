#include "LocationComponentDebugDrawer.h"
#include "Components/LocationComponent.h"
#include "CoreFramework/DebugRenderer.h"

void LocationComponentDebugDrawer::Draw(Component *comp)
{
    auto* loc = static_cast<LocationComponent*>(comp);
    ImGui::Text("Location: %s", loc->GetLocation().ToString().c_str());
}
