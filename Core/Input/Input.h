// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <unordered_map>
#include <SDL3/SDL_keycode.h>

#include "InputAction.h"
#include "Utilities/SingletonHelpers.h"

typedef std::vector<std::shared_ptr<InputActionBase>> ActionContainer;

class Input
{
public:
    static Input& Get()
    {
        return SingletonHelper::Impl<Input>();
    }

    void PushOntoInputBuffer(SDL_Keycode key, bool isKeyDown);
    void RegisterAction(std::shared_ptr<InputActionBase> action);
    void ProcessInputBuffer();
    
private:
    std::unordered_map<SDL_Keycode, ActionContainer> Actions;
    std::vector<std::pair<SDL_Keycode, bool>> InputBuffer;

    void ProcessInputEvent(SDL_Keycode key, bool isKeyDown);
};
