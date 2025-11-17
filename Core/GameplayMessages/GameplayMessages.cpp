#include "GameplayMessages.h"

void GameplayMessages::BroadcastMessage(const HashedString& channel, const GameplayMessage& message)
{
    auto channelIter = Channels.find(channel);
    if (channelIter == Channels.end())
        return;

    std::vector<std::function<void(const GameplayMessage&)>>& callbacks = channelIter->second.Callbacks;
    for (const auto& callback : callbacks)
    {
        callback(message);
    }
}

void GameplayMessages::SubscribeMessage(const HashedString& channel, const std::function<void(const GameplayMessage&)>& callback)
{
    if (!Channels.contains(channel))
    {
        Channels.emplace(channel, ChannelContainer<const GameplayMessage&>());
    }

    ContainerType& container = Channels[channel];
    container.Callbacks.emplace_back(callback);
}