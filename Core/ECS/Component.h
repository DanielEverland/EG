// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

class Component
{
};

// Strictly enforce empty size of base class
static_assert(sizeof(Component) == 1);
