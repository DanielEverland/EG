#include "gtest/gtest.h"
#include <SDL3/SDL_keycode.h>

#include "GameplayMessages/GameplayMessages.h"
#include "Input/Input.h"

HashedString ChannelA("ChannelA");

struct MessageAData : GameplayMessage
{
    uint8_t value;
};

struct MessageBData : GameplayMessage
{
    uint8_t value;
};

TEST(GameplayMessages, EntityMessage)
{
    auto& messages = GameplayMessages::Get();

    const Entity targetA = 0;
    const Entity targetB = 1;
    uint8_t callbacks = 0;

    messages.SubscribeEntityMessage(targetA, ChannelA, [&](Entity caller, const GameplayMessage& message)
    {
        callbacks++;
        EXPECT_EQ(targetA, caller);
        EXPECT_EQ(message.GetChecked<MessageAData>().value, 42);
        EXPECT_DEATH(message.GetChecked<MessageBData>(), ".*");
    });

    MessageAData inputData;
    inputData.value = 42;
    
    messages.BroadcastEntityMessage(targetA, ChannelA, inputData);
    messages.BroadcastEntityMessage(targetB, ChannelA, inputData);

    EXPECT_EQ(callbacks, 1);
}