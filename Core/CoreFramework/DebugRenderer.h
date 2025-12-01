// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <functional>

#include "imgui.h"

class DebugRenderer
{
public:
    static void Draw();

private:
    template<typename T, typename Comp>
    static ImVec4 GetRenderColor(T highThreshold, T mediumThreshold, T value, Comp comp)
    {
        if (comp(value, highThreshold))
            return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

        if (comp(value, mediumThreshold))
            return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

        return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
};
