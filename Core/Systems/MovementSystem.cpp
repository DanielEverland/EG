#include "MovementSystem.h"

#include "Components/LocationComponent.h"
#include "Components/MovementComponent.h"

void MovementSystem::Execute()
{
    auto gameMode = Game::Get().GetGameMode();
    auto level = Game::Get().GetLevel();
    Query<LocationComponent, MovementComponent>(
        [this, level](Entity entity, LocationComponent& location, MovementComponent& movement)
        {   
            for (movement.MovementSpeedRemainder += movement.MovementSpeed; movement.MovementSpeedRemainder >= 1.0f; movement.MovementSpeedRemainder -= 1.0f)
            {
                IntVector direction = movement.TargetLocation - location.GetLocation();
                direction.Normalize();
                
                const IntVector newPosition = location.GetLocation() + direction;
                if (!level->IsValidMove(entity, location.GetLocation(), newPosition))
                    continue;

                location.SetLocation(entity, newPosition);
            }
        });
}
