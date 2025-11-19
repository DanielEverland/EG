#include "MovementSystem.h"

#include "Components/CollisionComponent.h"
#include "Components/LocationComponent.h"
#include "Components/MovementComponent.h"

void MovementSystem::Execute()
{
    auto gameMode = Game::Get().GetGameMode();
    Query<LocationComponent, MovementComponent>(
        [this](Entity entity, LocationComponent& location, MovementComponent& movement)
        {   
            for (movement.MovementSpeedRemainder += movement.MovementSpeed; movement.MovementSpeedRemainder >= 1.0f; movement.MovementSpeedRemainder -= 1.0f)
            {
                IntVector direction = movement.TargetLocation - location.GetLocation();
                direction.Normalize();
                
                const IntVector newPosition = location.GetLocation() + direction;
                if (!IsValidMove(entity, location.GetLocation(), newPosition))
                    continue;

                location.SetLocation(entity, newPosition);
            }
        });
}
bool MovementSystem::IsValidMove(Entity entity, const IntVector& currentPosition, const IntVector& targetPosition) const
{
    return !Any<LocationComponent, CollisionComponent>([&targetPosition](const LocationComponent& location, CollisionComponent& _) -> bool
        {
           return location.GetLocation() == targetPosition;
        });
}
