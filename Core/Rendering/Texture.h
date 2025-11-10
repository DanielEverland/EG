// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <memory>

#include "Tileset.h"
#include "Primitives/Rect.h"

struct Texture
{
    Rect SourceRect;
    std::shared_ptr<Tileset> Tileset;
};
