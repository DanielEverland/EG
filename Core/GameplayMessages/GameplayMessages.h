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
    
public:
    static GameplayMessages& Get()
    {
        return SingletonHelper::Impl<GameplayMessages>();
    }

    void BroadcastMessage(const HashedString& channel, const GameplayMessage& message);
    void SubscribeMessage(const HashedString& channel, const std::function<void(const GameplayMessage&)>& callback);

private:
    using ContainerType = ChannelContainer<const GameplayMessage&>;
    std::unordered_map<HashedString, ContainerType> Channels;
};
