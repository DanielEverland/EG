#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <iostream>
#include <memory>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "../Core/Application.h"
#include "../Core/CoreFramework/EngineStatics.h"

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{    
    std::string bootDir = argv[0];
    bootDir = bootDir.substr(0, bootDir.find_last_of('\\') + 1);
    EngineStatics::BootDir = bootDir;
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("ECS Game", 1920, 1080, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    Renderer::Get().SetRenderer(renderer);
    Application::Get().Initialize();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    ImGui_ImplSDL3_ProcessEvent(event);

    if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP) {
        Application::Get().HandleInputEvent(event->key, event->type == SDL_EVENT_KEY_DOWN);
    }
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    
    auto& app = Application::Get();
    
    auto startTime = std::chrono::high_resolution_clock::now();
    app.Tick();
    app.PrePresent();
    auto gameCount = std::chrono::duration_cast< std::chrono::microseconds >( std::chrono::high_resolution_clock::now() - startTime );
    startTime = std::chrono::high_resolution_clock::now();
    Renderer::Get().Present();
    auto renderCount = std::chrono::duration_cast< std::chrono::microseconds >( std::chrono::high_resolution_clock::now() - startTime );
    
    app.GameTimeMs = static_cast<float>(gameCount.count()) / 1000.0f;
    app.RenderTimeMs = static_cast<float>(renderCount.count()) / 1000.0f;
    
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}
