// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <SDL3/SDL_events.h>

class InputReceiver
{
public:
    void OnInputEvent(const SDL_KeyboardEvent& keyEvent);
};
