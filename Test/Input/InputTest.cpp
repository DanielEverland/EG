#include "gtest/gtest.h"
#include <SDL3/SDL_keycode.h>
#include "Input/Input.h"

TEST(Input, SimpleDiscreteActions)
{
    auto input = Input::Get();

    auto action = std::make_shared<DiscreteInputAction>();
    action->AddKeycode(SDLK_A);
    action->AddKeycode(SDLK_B);

    uint8_t callbackCount = 0;
    action->AddCallback([&]()
    {
        callbackCount++;
    });

    input.RegisterAction(action);
    input.PushOntoInputBuffer(SDLK_A, true);
    input.PushOntoInputBuffer(SDLK_B, true);
    input.PushOntoInputBuffer(SDLK_C, true);
    input.ProcessInputBuffer();

    EXPECT_EQ(callbackCount, 2);
}

TEST(Input, DiscreteActionsFilters)
{
    auto input = Input::Get();

    auto action = std::make_shared<DiscreteInputAction>();
    action->AddKeycode(SDLK_A, InputEventType::KeyDown);
    action->AddKeycode(SDLK_B, InputEventType::KeyUp);

    uint8_t callbackCount = 0;
    action->AddCallback([&]()
    {
        callbackCount++;
    });

    input.RegisterAction(action);
    input.PushOntoInputBuffer(SDLK_A, true);
    input.PushOntoInputBuffer(SDLK_B, true);
    input.PushOntoInputBuffer(SDLK_C, true);
    input.ProcessInputBuffer();

    EXPECT_EQ(callbackCount, 1);
}

TEST(Input, SimpleAxisActions)
{
    auto input = Input::Get();

    auto action = std::make_shared<AxisInputAction>();
    action->AddKeycodeAxis(SDLK_A, 1);
    action->AddKeycodeAxis(SDLK_B, 2);

    uint8_t callbackCount = 0;
    uint8_t valuesReceived = 0;
    action->AddCallback([&](int32_t val)
    {
        callbackCount++;
        valuesReceived += val;
    });

    input.RegisterAction(action);
    input.PushOntoInputBuffer(SDLK_A, true);
    input.PushOntoInputBuffer(SDLK_B, true);
    input.PushOntoInputBuffer(SDLK_C, true);
    input.ProcessInputBuffer();

    EXPECT_EQ(callbackCount, 2);
    EXPECT_EQ(valuesReceived, 3);
}

TEST(Input, AxisActionsFilters)
{
    auto input = Input::Get();

    auto action = std::make_shared<AxisInputAction>();
    action->AddKeycodeAxis(SDLK_A, 1, InputEventType::KeyDown);
    action->AddKeycodeAxis(SDLK_B, 2, InputEventType::KeyUp);

    uint8_t callbackCount = 0;
    uint8_t valuesReceived = 0;
    action->AddCallback([&](int32_t val)
    {
        callbackCount++;
        valuesReceived += val;
    });

    input.RegisterAction(action);
    input.PushOntoInputBuffer(SDLK_A, true);
    input.PushOntoInputBuffer(SDLK_B, true);
    input.PushOntoInputBuffer(SDLK_C, true);
    input.ProcessInputBuffer();

    EXPECT_EQ(callbackCount, 1);
    EXPECT_EQ(valuesReceived, 1);
}