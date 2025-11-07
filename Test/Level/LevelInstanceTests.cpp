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
    l0.LoadData();
    

    // TODO: Didn't account for multiple tilesets.
    /*ASSERT_EQ(l0.GetSourceRectFromTileIdx(0), Rect(0, 0, 16, 24));
    ASSERT_EQ(l0.GetSourceRectFromTileIdx(7), Rect(112, 0, 16, 24));
    auto fa = l0.GetSourceRectFromTileIdx(34);
    ASSERT_EQ(l0.GetSourceRectFromTileIdx(34), Rect(32, 24, 16, 24));*/
}
