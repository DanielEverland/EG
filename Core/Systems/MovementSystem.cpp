#include "MovementSystem.h"

#include "Components/CollisionComponent.h"
#include "Components/LocationComponent.h"
#include "Components/MovementComponent.h"
#include "ECS/SystemOperation.h"

void MovementSystem::Execute()
{
    auto gameMode = Game::Get().GetGameMode();

    //AnyOperation<>
 
    QueryOperation<LocationComponent, const MovementComponent> op(this, [this](/*Entity entity, */LocationComponent& location, const MovementComponent& movement)
        {
            /*IntVector2D direction = movement.TargetLocation - location.WorldLocation;
            direction.Normalize();
            
            const IntVector2D newPosition = location.WorldLocation + direction;
            if (!IsValidMove(entity, location.WorldLocation, newPosition))
                return;

            location.WorldLocation = newPosition;*/
        });
    
    /*Query<LocationComponent, MovementComponent>(
        [this](Entity entity, LocationComponent& location, const MovementComponent& movement)
        {
            IntVector2D direction = movement.TargetLocation - location.WorldLocation;
            direction.Normalize();
            
            const IntVector2D newPosition = location.WorldLocation + direction;
            if (!IsValidMove(entity, location.WorldLocation, newPosition))
                return;

            location.WorldLocation = newPosition;
        });*/
}
bool MovementSystem::IsValidMove(Entity entity, const IntVector2D& currentPosition, const IntVector2D& targetPosition) const
{
    return false;
    /*return !Any<LocationComponent, CollisionComponent>([&targetPosition](const LocationComponent& location, CollisionComponent& _) -> bool
        {
           return location.WorldLocation == targetPosition;
        });*/
}

void MovementSystem::RegisterIterators()
{
    
    
    /*Query<LocationComponent, MovementComponent>(
        [this](Entity entity, LocationComponent& location, const MovementComponent& movement)
        {
            IntVector2D direction = movement.TargetLocation - location.WorldLocation;
            direction.Normalize();
            
            const IntVector2D newPosition = location.WorldLocation + direction;
            if (!IsValidMove(entity, location.WorldLocation, newPosition))
                return;

            location.WorldLocation = newPosition;
        });*/

    QueryOperation<LocationComponent, const MovementComponent> op(this, [this](/*Entity entity, */LocationComponent& location, const MovementComponent& movement)
       {
           /*IntVector2D direction = movement.TargetLocation - location.WorldLocation;
           direction.Normalize();
           
           const IntVector2D newPosition = location.WorldLocation + direction;
           if (!IsValidMove(entity, location.WorldLocation, newPosition))
               return;

           location.WorldLocation = newPosition;*/
       });
}