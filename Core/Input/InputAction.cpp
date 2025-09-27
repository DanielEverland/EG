#include "InputAction.h"

#include <algorithm>
#include <ranges>

void DiscreteInputAction::AddKeycode(SDL_Keycode keycode, uint8_t eventTypeFilter)
{
    Keys.push_back({
        .Keycode = keycode,
        .Filter = eventTypeFilter
    });
}

GenericHandle DiscreteInputAction::AddCallback(const std::function<void()>& callback)
{
    GenericHandle newHandle;
    Callbacks.emplace(newHandle, callback);
    return newHandle;
}

void DiscreteInputAction::RemoveCallback(GenericHandle handle)
{
    Callbacks.erase(handle);
}

void DiscreteInputAction::OnInputEvent(SDL_Keycode keycode, bool isDown)
{
    InputEventType::Type eventType = InputEventType::KeyHeld;
    if (isDown && !DownCodes.contains(keycode))
    {
        eventType = InputEventType::KeyDown;
        DownCodes.emplace(keycode);
    }
    else if (!isDown)
    {
        eventType = InputEventType::KeyUp;
        DownCodes.erase(keycode);
    }

    for (const KeycodeDiscrete& key : Keys)
    {
        if (key.Keycode == keycode && (key.Filter & eventType) != InputEventType::None)
        {
            Invoke();
        }
    }
}

std::vector<SDL_Keycode> DiscreteInputAction::GetRelevantKeycodes() const
{
    std::vector<SDL_Keycode> outKeys(Keys.size());
    for (const KeycodeDiscrete& key : Keys)
    {
        outKeys.push_back(key.Keycode);
    }
    return outKeys;
}

void DiscreteInputAction::Invoke()
{
    for (const std::function<void()>& callback : Callbacks | std::views::values)
    {
        callback();
    }
}

void AxisInputAction::AddKeycodeAxis(SDL_Keycode keycode, int32_t value, uint8_t eventTypeFilter)
{
    Axes.push_back({
            .Keycode = keycode,
            .Value = value,
            .Filter = eventTypeFilter
        });
}

GenericHandle AxisInputAction::AddCallback(const std::function<void(int32_t)>& callback)
{
    GenericHandle newHandle;
    Callbacks.emplace(newHandle, callback);
    return newHandle;
}

void AxisInputAction::RemoveCallback(GenericHandle handle)
{
    Callbacks.erase(handle);
}

void AxisInputAction::OnInputEvent(SDL_Keycode keycode, bool isDown)
{
    auto iter = std::ranges::find(DownCodes, keycode);
    InputEventType::Type eventType = InputEventType::KeyHeld;
    if (iter == DownCodes.end())
    {
        eventType = InputEventType::KeyDown;
        DownCodes.push_back(keycode);
    }
    else if (isDown == false)
    {
        eventType = InputEventType::KeyUp;
        DownCodes.erase(iter);
    }

    for (const KeycodeAxis& axis : Axes)
    {
        if (axis.Keycode == keycode && (axis.Filter & eventType) != InputEventType::None)
        {
            Invoke(axis.Value);
        }
    }
}

std::vector<SDL_Keycode> AxisInputAction::GetRelevantKeycodes() const
{
    std::vector<SDL_Keycode> outKeys(Axes.size());
    for (const KeycodeAxis& key : Axes)
    {
        outKeys.push_back(key.Keycode);
    }
    return outKeys;
}

void AxisInputAction::Invoke(int32_t value)
{
    for (const std::function<void(int)>& callback : Callbacks | std::ranges::views::values)
    {
        callback(value);
    }
}
