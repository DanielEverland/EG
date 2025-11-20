#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <iostream>
#include <memory>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "../Core/Application.h"
#include "../Core/CoreFramework/EngineStatics.h"

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{    
    std::string bootDir = argv[0];
    bootDir = bootDir.substr(0, bootDir.find_last_of('\\') + 1);
    EngineStatics::BootDir = bootDir;
    
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", 640, 480, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    Renderer::Get().SetRenderer(renderer);
    Application::Get().Initialize();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP) {
        Application::Get().HandleInputEvent(event->key, event->type == SDL_EVENT_KEY_DOWN);
    }
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    auto app = Application::Get();
    app.Tick();
    app.PrePresent();
    Renderer::Get().Present();

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
