// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <functional>
#include <set>
#include <SDL3/SDL_keycode.h>

#include "GenericHandle.h"
#include "InputEventType.h"

enum class InputActionNames
{
    None = 0,

    MoveHorizontal,
    MoveVertical
};

class InputActionBase
{
public:
    virtual ~InputActionBase() = default;

    virtual void OnInputEvent(SDL_Keycode keycode, bool isDown) = 0;
    virtual std::vector<SDL_Keycode> GetRelevantKeycodes() const = 0;
};

class DiscreteInputAction : public InputActionBase
{
    struct KeycodeDiscrete
    {
        SDL_Keycode Keycode;
        uint8_t Filter = InputEventType::Any;
    };
    
public:
    void AddKeycode(SDL_Keycode keycode, uint8_t eventTypeFilter = InputEventType::Any);
    GenericHandle AddCallback(const std::function<void()>& callback);
    void RemoveCallback(GenericHandle handle);
    
    void OnInputEvent(SDL_Keycode keycode, bool isDown) override;
    std::vector<SDL_Keycode> GetRelevantKeycodes() const override;

private:
    std::unordered_map<GenericHandle, std::function<void()>> Callbacks;
    std::set<SDL_Keycode> DownCodes;
    std::vector<KeycodeDiscrete> Keys;

    void Invoke();
};

class AxisInputAction : public InputActionBase
{
    struct KeycodeAxis
    {
        SDL_Keycode Keycode;
        int32_t Value;
        uint8_t Filter = InputEventType::Any;
    };
    
public:
    void AddKeycodeAxis(SDL_Keycode keycode, int32_t value, uint8_t eventTypeFilter = InputEventType::Any);
    GenericHandle AddCallback(const std::function<void(int32_t)>& callback);
    void RemoveCallback(GenericHandle handle);

    void OnInputEvent(SDL_Keycode keycode, bool isDown) override;
    std::vector<SDL_Keycode> GetRelevantKeycodes() const override;

private:
    std::unordered_map<GenericHandle, std::function<void(int32_t)>> Callbacks;
    std::vector<KeycodeAxis> Axes;
    std::vector<SDL_Keycode> DownCodes;

    void Invoke(int32_t value);
};