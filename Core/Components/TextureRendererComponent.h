// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "DataStructrues/HashedString.h"
#include "CoreFramework/Renderer.h"
#include "ECS/Component.h"
#include "CoreFramework/DebugRenderer.h"
#include "Debugging/TextureRendererComponentDebugDrawer.h"
#include "Components/Factories/TextureRendererFactory.h"

REGISTER_COMP_FACTORY(TextureRendererFactory, "TextureRendererComponent")

class TextureRendererComponent : public Component
{
public:    
    HashedString TextureName { "Default" };
    DrawCallOrder Order = DrawCallOrder::Foreground;
};

REGISTER_DEBUGGER(TextureRendererComponent, TextureRendererComponentDebugDrawer)
