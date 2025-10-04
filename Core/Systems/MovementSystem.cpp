#include "MovementSystem.h"

#include "Components/CollisionComponent.h"
#include "Components/LocationComponent.h"
#include "Components/MovementComponent.h"

void MovementSystem::Execute()
{
    auto gameMode = Game::Get().GetGameMode();
    Query<LocationComponent, MovementComponent>(
        [this](Entity entity, LocationComponent& location, const MovementComponent& movement)
        {
            IntVector2D direction = movement.TargetLocation - location.WorldLocation;
            direction.Normalize();
            
            const IntVector2D newPosition = location.WorldLocation + direction;
            if (!IsValidMove(entity, location.WorldLocation, newPosition))
                return;

            location.WorldLocation = newPosition;
        });
}
bool MovementSystem::IsValidMove(Entity entity, const IntVector2D& currentPosition, const IntVector2D& targetPosition) const
{
    return !Any<LocationComponent, CollisionComponent>([&targetPosition](const LocationComponent& location, CollisionComponent& _) -> bool
        {
           return location.WorldLocation == targetPosition;
        });
}
