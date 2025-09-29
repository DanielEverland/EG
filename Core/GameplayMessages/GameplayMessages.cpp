#include "GameplayMessages.h"

void GameplayMessages::BroadcastEntityMessage(Entity caller, const HashedString& channel, const GameplayMessage& message)
{
    auto containerIter = EntitySubscriptions.find(caller);
    if (containerIter == EntitySubscriptions.end())
        return;

    auto channelIter = containerIter->second.Channels.find(channel);
    if (channelIter == containerIter->second.Channels.end())
        return;

    std::vector<std::function<void(unsigned, const GameplayMessage&)>>& callbacks = channelIter->second.Callbacks;
    for (const auto& callback : callbacks)
    {
        callback(caller, message);
    }
}

void GameplayMessages::SubscribeEntityMessage(Entity target, const HashedString& channel,
    const std::function<void(Entity, const GameplayMessage&)>& callback)
{
    if (!EntitySubscriptions.contains(target))
    {
        EntitySubscriptions.emplace(target, EntityContainer());
    }

    EntityContainer& subscription = EntitySubscriptions[target];
    if (!subscription.Channels.contains(channel))
    {
        subscription.Channels.emplace(channel, EntityContainer::ContainerType());
    }

    EntityContainer::ContainerType& container = subscription.Channels[channel];
    container.Callbacks.emplace_back(callback);
}