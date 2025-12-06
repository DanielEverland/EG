#include "DebugRenderer.h"

#include <algorithm>

#include "Application.h"
#include "GameSettings.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Level.h"
#include "Logging/Logger.h"

void DebugRenderer::Draw()
{
    DrawFrameTimeGraph();
    DrawDevMenu();
}

void DebugRenderer::RegisterDebugDrawer(std::type_index targetIdx, std::unique_ptr<DebugDrawer> drawer)
{
    Drawers[targetIdx] = std::move(drawer);
}

void DebugRenderer::DrawFrameTimeGraph()
{
    constexpr int32_t HistogramSize = 100;
    static std::array<float, HistogramSize> FPSHistory;
    static int32_t FPSHistoryIdx = 0;
    
    constexpr float HighMsThreshold = 1.0f/60*1000;
    constexpr float MediumMsThreshold = 1.0f/30*1000;
    
    auto& app = Application::Get();

    auto viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("DebugRenderer", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);

    ImGui::TextColored(GetRenderColor(HighMsThreshold, MediumMsThreshold, app.GameTimeMs, std::less()),     "GameTime: %.2fms", app.GameTimeMs);
    ImGui::TextColored(GetRenderColor(HighMsThreshold, MediumMsThreshold, app.RenderTimeMs, std::less()),   "RenderTime: %.2fms", app.RenderTimeMs);

    // FPS
    float totalTime = app.GameTimeMs + app.RenderTimeMs;
    float fps = totalTime > 0 ? 1.0f / totalTime * 1000.0f : 0.0f;
    FPSHistory[FPSHistoryIdx] = fps;
    FPSHistoryIdx = ++FPSHistoryIdx % HistogramSize;
    ImGui::TextColored(GetRenderColor(60.0f, 30.0f, fps, std::greater_equal()),     "FPS: %.2f", fps);
    
    float maxElement = *std::ranges::max_element(FPSHistory);
    ImGui::PlotLines("##DebugRendererFPSHistogram", FPSHistory.data(), HistogramSize, FPSHistoryIdx, NULL, 0, maxElement, ImVec2(200, 100));
    
    ImGui::End();
}

void DebugRenderer::DrawDevMenu()
{
    static bool bHasInitialized = false;
    static bool bShowDemoWindow = false;

    if (!bHasInitialized)
    {
        bHasInitialized = true;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->Size.x - 550, viewport->WorkPos.y));
        ImGui::SetNextWindowSize(ImVec2(550, 680));
        ImGui::SetNextWindowCollapsed(false);
    }
    
    if (!ImGui::Begin("Developer Menu", nullptr, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // MENU BAR
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::Button("Imgui Demo"))
            {
                bShowDemoWindow = !bShowDemoWindow;
            }

            ImGui::EndMenuBar();
        }
    }

    if (bShowDemoWindow)
    {
        ImGui::ShowDemoWindow(&bShowDemoWindow);
    }

    // TABS
    {
        if (ImGui::BeginTabBar("DevMenuTabBar"))
        {
            if (ImGui::BeginTabItem("Entities"))
            {
                DrawEntitiesTabContent();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Rendering"))
            {
                ImGui::Checkbox("Draw Dirty Cells", &GameSettings::Get().DebugDrawRenderedCells);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }        
    }
    
    ImGui::End();
}

void DebugRenderer::DrawEntitiesTabContent()
{
    std::shared_ptr<Level> level = Game::Get().GetLevel();

    ImGui::Checkbox("Debug Under Cursor", &DebugEntitiesUnderCursor);
    if (DebugEntitiesUnderCursor)
    {
        if (!ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        {
            CursorPosition = ImGui::GetMousePos();
        }
        Vector mousePosInWorld = WorldPositionUtility::ScreenPositionToWorld(Vector2D(CursorPosition.x, CursorPosition.y));
        IntVector mousePosInCell = WorldPositionUtility::WorldPositionToCellPosition(mousePosInWorld);
        IntVector chunkPos = WorldPositionUtility::WorldPositionToChunkPosition(mousePosInCell);
        IntVector chunkSpace = WorldPositionUtility::WorldSpaceToChunkSpace(mousePosInCell);

        ImVec2 renderPos = CursorPosition;
        ImGuiWindow* existingWindow = ImGui::FindWindowByName("##DebugRenderer::DrawEntitiesTabContent_MouseCursorDebug");
        if (existingWindow != nullptr)
        {
            auto viewport = ImGui::GetMainViewport();
            if (renderPos.y + existingWindow->Size.y > viewport->Size.y)
            {
                renderPos.y -= (existingWindow->Size.y + renderPos.y) - viewport->Size.y;
            }
            else if (renderPos.y < 0)
            {
                renderPos.y = 0;
            }

            if (renderPos.x + existingWindow->Size.x > viewport->Size.x)
            {
                renderPos.x -= (existingWindow->Size.x + renderPos.x) - viewport->Size.x;
            }
            else if (renderPos.x < 0)
            {
                renderPos.x = 0;
            }
        }
        ImGui::SetNextWindowPos(renderPos);
        

        ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;
        flags &= ~(ImGuiWindowFlags_NoBackground);
        ImGui::Begin("##DebugRenderer::DrawEntitiesTabContent_MouseCursorDebug", nullptr, flags);

        ImGui::TextDisabled("Hold down CTRL to interact with window");
        ImGui::Text("Mouse World Pos: (%.2f, %.2f, %.2f)", mousePosInWorld.X, mousePosInWorld.Y, mousePosInWorld.Z);
        ImGui::Text("Cell Pos: %s", mousePosInCell.ToString().c_str());

        ImGui::Text("Chunk: %s", chunkPos.ToString().c_str());
        ImGui::SameLine();
        ImGui::Text("Chunk Pos: %s", chunkSpace.ToString().c_str());

        ImGui::Separator();

        const auto& container = level->GetEntitiesAtPosition(mousePosInCell);
        for (Entity entity : container.Entities)
        {
            DrawEntityFoldout(entity);
        }

        ImGui::End();
    }

    if (ImGui::CollapsingHeader("Chunks"))
    {
        std::vector<IntVector> chunks = level->GetLoadedChunks();
        for (const IntVector& chunkPos : chunks)
        {
            std::string treeId = std::format("Chunk {}##DebugRenderer::DrawEntitiesTabContent", chunkPos.ToString().c_str());
            if (ImGui::TreeNode(treeId.c_str()))
            {
                ImGui::TreePop();
            }
        }
    }
}

void DebugRenderer::DrawEntityFoldout(Entity entity)
{
    std::shared_ptr<Level> level = Game::Get().GetLevel();
    
    std::string id = std::format("Entity ID({})##DebugRenderer::DrawEntityFoldout", entity);
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader(id.c_str(), nullptr))
    {
        std::vector<Component*> comps = level->GetComponentManager().GetAllComponents(entity);
        ImGui::SeparatorText(std::format("Components [{}]##DebugRenderer::DrawEntityFoldout_{}", comps.size(), entity).c_str());
        for (Component* comp : comps)
        {
            auto treeId = std::format("{}##{}{}", typeid(*comp).name(), entity, typeid(*comp).hash_code());
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            if (ImGui::TreeNode(treeId.c_str()))
            {
                auto iter = Drawers.find(typeid(*comp));
                if (iter != Drawers.end())
                {
                    iter->second->Draw(comp);
                }
                else
                {
                    ImGui::Text("NO DRAWER");
                }
                ImGui::TreePop();
            }
        }
    }
}