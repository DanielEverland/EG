#include "Input.h"

void Input::PushOntoInputBuffer(const SDL_Keycode key, bool isKeyDown)
{
    InputBuffer.push_back({key, isKeyDown});
}

void Input::RegisterAction(std::shared_ptr<InputActionBase> action)
{
    std::vector<SDL_Keycode> keys = action->GetRelevantKeycodes();
    for (const SDL_Keycode& key : keys)
    {
        if (!Actions.contains(key))
        {
            Actions.emplace(key, std::vector<std::shared_ptr<InputActionBase>>());    
        }
        Actions[key].push_back(action);
    }
}
void Input::ProcessInputBuffer()
{
    for (auto element : InputBuffer)
    {
        ProcessInputEvent(element.first, element.second);
    }
    InputBuffer.clear();
}

void Input::ProcessInputEvent(SDL_Keycode key, bool isKeyDown)
{
    for (auto element : Actions)
    {
        if (element.first == key)
        {
            for (const std::shared_ptr<InputActionBase> action : element.second)
            {
                action->OnInputEvent(key, isKeyDown);
            }
        }
    }

    // TODO: Fix this to use find
    /*auto iter = std::find(Actions.begin(), Actions.end(), static_cast<uint32_t>(key));
    if (iter == Actions.end())
        return;
    
    for (const std::shared_ptr<InputActionBase> element : iter->second)
    {
        element->OnInputEvent(key, isKeyDown);
    }*/
}
