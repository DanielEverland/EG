// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <cstdint>

namespace InputEventType
{
    enum Type : uint8_t
    {
        None = 0,
        KeyDown = 1 << 0,
        KeyUp = 1 << 1,
        KeyHeld = 1 << 2,

        DownOrHeld = KeyDown | KeyHeld,
        Any = KeyDown | KeyUp | KeyHeld
    };
}
