#include "InputAction.h"

#include <algorithm>
#include <ranges>

void DiscreteInputAction::AddKeycode(SDL_Keycode keycode)
{
    Keys.push_back(keycode);
}

GenericHandle DiscreteInputAction::AddCallback(const std::function<void(bool)>& callback)
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
    if (isDown)
    {
        DownCodes.emplace(keycode);
    }
    else
    {
        DownCodes.erase(keycode);
    }

    Invoke(DownCodes.empty() == false);
}

std::vector<SDL_Keycode> DiscreteInputAction::GetRelevantKeycodes() const
{
    return Keys;
}

void DiscreteInputAction::Invoke(const bool isDown)
{
    for (const std::function<void(bool)>& callback : Callbacks | std::views::values)
    {
        callback(isDown);
    }
}

void AxisInputAction::SetKeycodeAxis(SDL_Keycode keycode, int32_t value, uint8_t eventTypeFilter)
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
    for (auto pair : Axes)
    {
        outKeys.push_back(pair.Keycode);
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
