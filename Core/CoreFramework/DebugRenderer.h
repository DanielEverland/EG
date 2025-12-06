// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <typeindex>
#include <unordered_map>
#include <memory>

#include "imgui.h"
#include "ECS/Entity.h"

class DebugDrawer;

class DebugRenderer
{
public:
    static void Draw();
    static void RegisterDebugDrawer(std::type_index targetIdx, std::unique_ptr<DebugDrawer> drawer);

private:
    static inline bool DebugEntitiesUnderCursor = false;
    static inline std::unordered_map<std::type_index, std::unique_ptr<DebugDrawer>> Drawers = { };
    
    template<typename T, typename Comp>
    static ImVec4 GetRenderColor(T highThreshold, T mediumThreshold, T value, Comp comp)
    {
        if (comp(value, highThreshold))
            return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

        if (comp(value, mediumThreshold))
            return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

        return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    }

    static void DrawFrameTimeGraph();
    static void DrawDevMenu();
    static void DrawEntitiesTabContent();
    static void DrawEntityFoldout(Entity entity);
};

template <class Target, class Debugger>
struct ComponentDebuggerRegistrar {
    ComponentDebuggerRegistrar()
    {
        std::unique_ptr<Debugger> ptr = std::make_unique<Debugger>();
        DebugRenderer::RegisterDebugDrawer(typeid(Target), std::move(ptr));
    }
};

#define REGISTER_DEBUGGER(TARGET, DEBUGGER) \
inline ComponentDebuggerRegistrar<TARGET, DEBUGGER> _AUTOGEN_##TARGET_##DEBUGGER;