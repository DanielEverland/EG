// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <assert.h>
#include <functional>

#include "DataStructrues/HashedString.h"
#include "ECS/Entity.h"
#include "Utilities/SingletonHelpers.h"

struct GameplayMessage
{
    virtual ~GameplayMessage() = default;
    
    template<class T>
    const T& GetChecked() const
    {
        assert(typeid(T) == typeid(*this) && "GameplayMessage::GetChecked - Invalid type cast");
        return *dynamic_cast<const T*>(this);
    }
};

class GameplayMessages
{
    template<class... T>
    struct ChannelContainer
    {
        std::vector<std::function<void(T ...)>> Callbacks;
    };

    struct EntityContainer
    {
        using ContainerType = ChannelContainer<Entity, const GameplayMessage&>;
        std::unordered_map<HashedString, ContainerType> Channels;
    };
    
public:
    static GameplayMessages& Get()
    {
        return SingletonHelper::Impl<GameplayMessages>();
    }

    void BroadcastEntityMessage(Entity caller, const HashedString& channel, const GameplayMessage& message);
    void SubscribeEntityMessage(Entity target, const HashedString& channel,
        const std::function<void(Entity, const GameplayMessage&)>& callback);

private:
    std::unordered_map<Entity, EntityContainer> EntitySubscriptions;
};
