// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cstdint>

class Color
{
public:
    Color() = default;
    Color(const uint8_t R, const uint8_t G, const uint8_t B) : R(R), G(G), B(B) { }
    
    uint8_t R;
    uint8_t G;
    uint8_t B;
};
