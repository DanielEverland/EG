#include <optional>

#include "DataStructrues/Vector.h"
#include "gtest/gtest.h"
#include "Utilities/ConversionUtil.h"
#include "Utilities/WorldPositionUtility.h"

TEST(WorldPositionUtilTest, WorldPosToChunkPos)
{
    {
        ASSERT_EQ(WorldPositionUtility::WorldPositionToChunkPosition(IntVector(0, 0, 0)), IntVector(0, 0, 0));
    }
    {
        ASSERT_EQ(WorldPositionUtility::WorldPositionToChunkPosition(IntVector(1, 0, 0)), IntVector(0, 0, 0));
    }
    {
        ASSERT_EQ(WorldPositionUtility::WorldPositionToChunkPosition(IntVector(WorldPositionUtility::ChunkWidth, 0, 0)), IntVector(1, 0, 0));
    }
    {
        ASSERT_EQ(WorldPositionUtility::WorldPositionToChunkPosition(IntVector(-1, 0, 0)), IntVector(-1, 0, 0));
    }
}

TEST(WorldPositionUtilTest, WorldToChunkSpace)
{
    {
        IntVector worldPos = IntVector(1, 1, 0);
        ASSERT_EQ(WorldPositionUtility::WorldSpaceToChunkSpace(worldPos), IntVector(1, 1, 0));
    }
    {
        IntVector worldPos = IntVector(0, 0, 0);
        ASSERT_EQ(WorldPositionUtility::WorldSpaceToChunkSpace(worldPos), IntVector(0, 0, 0));
    }
    {
        IntVector worldPos = IntVector(WorldPositionUtility::ChunkWidth, WorldPositionUtility::ChunkHeight, 0);
        ASSERT_EQ(WorldPositionUtility::WorldSpaceToChunkSpace(worldPos), IntVector(0, 0, 0));
    }
    {
        IntVector worldPos = IntVector(WorldPositionUtility::ChunkWidth + 1, WorldPositionUtility::ChunkHeight + 1, 0);
        ASSERT_EQ(WorldPositionUtility::WorldSpaceToChunkSpace(worldPos), IntVector(1, 1, 0));
    }
    {
        ASSERT_EQ(WorldPositionUtility::WorldSpaceToChunkSpace(IntVector(-1, 0, 0)), IntVector(WorldPositionUtility::ChunkWidth - 1, 0, 0));
    }
}