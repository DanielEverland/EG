#include "TestComponentA.h"
#include "TestComponentB.h"
#include "ECS/ComponentIterator.h"
#include "ECS/ComponentManager.h"
#include "gtest/gtest.h"

TEST(Component, SimpleIteration)
{
    ComponentManager manager;
    const Entity entity = 0;
    
    manager.AddComponent<TestComponentA>(entity);
    manager.AddComponent<TestComponentB>(entity);
    
    auto iter = manager.CreateIterator<TestComponentA, TestComponentB>();
    int32_t executions = 0;
    iter.Execute([&executions](TestComponentA & test_component_a, TestComponentB & test_component_b)
    {
        executions++;
        
        test_component_a.A = 1;
        test_component_a.B = 3.14;

        test_component_b.C = 1.1;
        test_component_b.D = 42.0;
        test_component_b.E = 2.2;
    });

    iter.Execute([&executions](TestComponentA & test_component_a, TestComponentB & test_component_b)
    {
        executions++;
        
        EXPECT_EQ(test_component_a.A, 1);
        EXPECT_DOUBLE_EQ(test_component_a.B, 3.14);

        EXPECT_DOUBLE_EQ(test_component_b.C, 1.1);
        EXPECT_DOUBLE_EQ(test_component_b.D, 42);
        EXPECT_DOUBLE_EQ(test_component_b.E, 2.2);
    });

    EXPECT_EQ(executions, 2);
}

TEST(Component, ProperSubset)
{
    ComponentManager manager;
    const Entity entityA = 0;
    const Entity entityB = 1;

    manager.AddComponent<TestComponentA>(entityA);
    manager.AddComponent<TestComponentA>(entityB);
    
    manager.AddComponent<TestComponentB>(entityA);

    int32_t executions = 0;
    auto iter = manager.CreateIterator<Entity, TestComponentA, TestComponentB>();
    iter.Execute([&executions, entityA](Entity entityId, TestComponentA&, TestComponentB&)
    {
        executions++;
        EXPECT_EQ(entityId, entityA);
    });

    EXPECT_EQ(executions, 1);
}