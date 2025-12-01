#include "DebugRenderer.h"

#include "Application.h"
#include "imgui.h"

void DebugRenderer::Draw()
{
    constexpr int32_t HistogramSize = 100;
    static std::array<float, HistogramSize> FPSHistory;
    static int32_t FPSHistoryIdx = 0;
    
    constexpr uint16_t HighMsThreshold = static_cast<uint16_t>(1.0f/60*1000);
    constexpr uint16_t MediumMsThreshold = static_cast<uint16_t>(1.0f/30*1000);
    
    auto& app = Application::Get();

    auto viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("DebugRenderer", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
    
    ImGui::TextColored(GetRenderColor(HighMsThreshold, MediumMsThreshold, app.GameTimeMs, std::less()),     "GameTime: %dms", app.GameTimeMs);
    ImGui::TextColored(GetRenderColor(HighMsThreshold, MediumMsThreshold, app.RenderTimeMs, std::less()),   "RenderTime: %dms", app.RenderTimeMs);

    // FPS
    uint16_t totalTime = app.GameTimeMs + app.RenderTimeMs;
    float fps = totalTime > 0 ? 1.0f / static_cast<float>(totalTime) * 1000.0f : 0.0f;
    FPSHistory[FPSHistoryIdx] = fps;
    FPSHistoryIdx++;
    if (FPSHistoryIdx >= HistogramSize) FPSHistoryIdx = 0;
    ImGui::TextColored(GetRenderColor(60.0f, 30.0f, fps, std::greater_equal()),     "FPS: %.2f", fps);
    
    static float MaxFPS = fps;
    if (fps > MaxFPS)
        MaxFPS = fps;
    ImGui::PlotLines("##DebugRendererFPSHistogram", FPSHistory.data(), HistogramSize, FPSHistoryIdx, NULL, 0, MaxFPS, ImVec2(100, 100));
    
    ImGui::End();
}
