// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "DataStructrues/HashedString.h"
#include "CoreFramework/Renderer.h"

class TextureRendererComponent : public Component
{
public:
    using Component::Component;
    
    HashedString TextureName { "Default" };
    DrawCallOrder Order = DrawCallOrder::Foreground;
};
