#include "TestComponentA.h"
#include "TestComponentB.h"
#include "Components/LocationComponent.h"
#include "Components/MovementComponent.h"
#include "ECS/ComponentManager.h"
#include "ECS/System.h"
#include "ECS/SystemOperation.h"
#include "ECS/SystemScheduler.h"
#include "gtest/gtest.h"

class TestSystem : public System
{
public:
    TestSystem()
    {
        op = QueryOperation<LocationComponent, const MovementComponent>(this, [this](/*Entity entity, */LocationComponent& location, const MovementComponent& movement)
        {
            
        });
    }
    
    QueryOperation<LocationComponent, const MovementComponent> op;

    void Execute() override
    {
        
    }
};

TEST(SystemScheduler, Operations)
{
    Game::Get().Initialize();
    
    SystemScheduler scheduler;

    auto system = std::make_shared<TestSystem>();
    
    scheduler.Register(system);
    while (true)
    {
        scheduler.Tick();
    }
    
    /*scheduler.Tick();
    scheduler.Tick();
    scheduler.Tick();*/
}