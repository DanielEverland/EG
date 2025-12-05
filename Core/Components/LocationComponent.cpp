// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#include "LocationComponent.h"
#include "GameplayMessages/GameplayMessages.h"
#include "GameplayMessages/LocationChangedMessage.h"

void LocationComponent::SetLocation(Entity owner, const IntVector& newLocation)
{
    LocationChangedMessage message;
    message.Owner = owner;
    message.OldLocation = WorldLocation;
    message.NewLocation = newLocation;

    WorldLocation = newLocation;
    
    GameplayMessages::Get().BroadcastMessage(HashedString("EntityLocationChanged"), message);
}
