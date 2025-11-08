#include "../Utility/TestDirectoryHelper.h"
#include "DataStructrues/HashedString.h"
#include "DataStructrues/Vector.h"
#include "gtest/gtest.h"
#include "Input/Input.h"
#include "Levels/LevelInstance.h"

class LevelTest : public testing::Test
{
protected:
    LevelTest()
    {
        std::filesystem::path p = TestDirectoryHelper::GetContentPath("TestLevel_01");
        std::string pString = p.generic_string();
        l0 = LevelInstance(pString);
    }

    LevelInstance l0;
};

TEST_F(LevelTest, Deserialization)
{
    size_t tileSetHash = std::hash<std::string>()("Environment.tsx");
    
    l0.LoadData();

    auto origoPos = IntVector(0, 0, 0);
    auto origoInfo = l0.GetCellInfoFromWorldPosition(origoPos);
    auto origoRect = l0.GetSourceRectFromWorldPosition(origoPos);
    ASSERT_EQ(origoInfo.CellTypeName, HashedString("Dirt"));
    ASSERT_EQ(origoInfo.TileId, 0);
    ASSERT_EQ(origoInfo.TileSetId, tileSetHash);
    ASSERT_EQ(origoRect, Rect(0, 0, 16, 24));

    auto horizontalWallPos = IntVector(5, 8, 0);
    auto horizontalWallInfo = l0.GetCellInfoFromWorldPosition(IntVector(horizontalWallPos));
    auto horizontalWallRect = l0.GetSourceRectFromWorldPosition(IntVector(horizontalWallPos));
    ASSERT_EQ(horizontalWallInfo.CellTypeName, HashedString("Wall"));
    ASSERT_EQ(horizontalWallInfo.TileId, 67);
    ASSERT_EQ(horizontalWallInfo.TileSetId, tileSetHash);
    ASSERT_EQ(horizontalWallRect, Rect(48, 48, 16, 24));

    auto cornerWallPos = IntVector(3, 8, 0);
    auto cornerWallInfo = l0.GetCellInfoFromWorldPosition(IntVector(cornerWallPos));
    auto cornerWallRect = l0.GetSourceRectFromWorldPosition(IntVector(cornerWallPos));
    ASSERT_EQ(cornerWallInfo.CellTypeName, HashedString("Wall"));
    ASSERT_EQ(cornerWallInfo.TileId, 71);
    ASSERT_EQ(cornerWallInfo.TileSetId, tileSetHash);
    ASSERT_EQ(cornerWallRect, Rect(112, 48, 16, 24));
}