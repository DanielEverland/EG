#include "gtest/gtest.h"
#include "DataStructrues/SparseSet.h"

TEST(SparseSet, Defaults)
{
    SparseSet<int> set;
    Entity entityId = 0;
    
    EXPECT_EQ(set.Size(), 0);
    EXPECT_FALSE(set.Contains(entityId));
}

TEST(SparseSet, Modifying)
{
    SparseSet<int> set;
    const Entity entityId = 0;

    // Adding
    set.AddDefaulted(entityId);
    EXPECT_EQ(set.Size(), 1);
    EXPECT_TRUE(set.Contains(entityId));

    // Duplicates not possible
    set.AddDefaulted(entityId);
    EXPECT_EQ(set.Size(), 1);
    EXPECT_TRUE(set.Contains(entityId));

    // Removing
    set.Remove(entityId);
    EXPECT_EQ(set.Size(), 0);
    EXPECT_FALSE(set.Contains(entityId));

    // Removing empty is ok
    set.Remove(entityId);
}

TEST(SparseSet, SatelliteData)
{
    SparseSet<int> set;
    const Entity entityId = 0;

    int& data = set.AddDefaulted(entityId);
    EXPECT_EQ(data, 0);
    data = 5;
    EXPECT_EQ(set.Get(entityId), 5);

    // Const-context
    const SparseSet<int>& constSet = set;
    const int& constData = constSet.Get(entityId);
    EXPECT_EQ(constData, 5);
}