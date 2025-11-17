#include <unordered_map>

#include "CoreFramework/Navigation.h"
#include "gtest/gtest.h"

#include "CoreFramework/NavigationGraphTraverser.h"
#include "CoreFramework/NavRequest.h"

class TestNavigationGraphTraverser : public NavigationGraphTraverser
{
public:
    bool DoesWorldPositionExist(const IntVector& worldPosition) override
    {
        return worldPosition.X >= 0 && worldPosition.Y >= 0 && worldPosition.Z == 0
            && worldPosition.X < Size.X && worldPosition.Y < Size.Y;
    }
    
    bool TryGetTraversalDifficulty(const IntVector& from, const IntVector& to, uint8_t& outTraversalDifficulty) override
    {
        auto iter = Tiles.find(to);
        if (iter == Tiles.end())
            return false;
        outTraversalDifficulty = iter->second;
        return true;
    }

    std::unordered_map<IntVector, uint8_t> Tiles;
    IntVector2D Size = IntVector2D(32, 32);
};

class NavigationTest : public testing::Test
{
protected:
    NavigationTest()
    {
        ///
        ///     FLAT LEVEL
        ///     Completely flat with no terrain costs or obstacles
        ///
        FlatLevelNoDifficulty = std::make_shared<TestNavigationGraphTraverser>();
        for (int i = 0; i < 32; ++i)
        {
            for (int j = 0; j < 32; ++j)
            {
                FlatLevelNoDifficulty->Tiles.emplace(IntVector(i, j, 0), 1);
            }
        }

        ///
        ///     CONCAVE LEVEL
        ///     Completely flat with no terrain costs, but does have a concave room similar to
        ///     Stanford literature examples:
        ///     https://theory.stanford.edu/~amitp/GameProgramming/AStarComparison.html
        ///
        ConcaveLevel = std::make_shared<TestNavigationGraphTraverser>();
        for (int i = 0; i < 32; ++i)
        {
            for (int j = 0; j < 32; ++j)
            {
                ConcaveLevel->Tiles.emplace(IntVector(i, j, 0), 1);
            }
        }
        for (int i = 5; i < 20; ++i)
        {
            ConcaveLevel->Tiles.erase(IntVector(i, 5, 0));
            ConcaveLevel->Tiles.erase(IntVector(i, 19, 0));
            ConcaveLevel->Tiles.erase(IntVector(20, i, 0));
        }

        ///
        ///     LEVEL WITH ROAD
        ///     Has a road and a concave obstacle
        ///     Stanford literature examples:
        ///     https://theory.stanford.edu/~amitp/GameProgramming/AStarComparison.html
        ///
        RoadLevel = std::make_shared<TestNavigationGraphTraverser>();
        for (int i = 0; i < 32; ++i)
        {
            for (int j = 0; j < 32; ++j)
            {
                RoadLevel->Tiles.emplace(IntVector(i, j, 0), j == 5 || i == 15 ? 1 : 10);
            }
        }
    }

    std::shared_ptr<TestNavigationGraphTraverser> FlatLevelNoDifficulty;
    std::shared_ptr<TestNavigationGraphTraverser> ConcaveLevel;
    std::shared_ptr<TestNavigationGraphTraverser> RoadLevel;
    Navigation Nav;
};

TEST_F(NavigationTest, Simple)
{
    NavRequest request;
    request.StartPosition = IntVector(0, 0, 0);
    request.TargetPosition = IntVector(3, 0, 0);
    request.Traverser = FlatLevelNoDifficulty;

    std::list expectedPath = {
        IntVector(0, 0, 0),
        IntVector(1, 0, 0),
        IntVector(2, 0, 0),
        IntVector(3, 0, 0),
    };

    NavResult result = Nav.TryCalculatePath(request);

    ASSERT_TRUE(result.Succeeded);
    ASSERT_EQ(result.Path.size(), expectedPath.size());
    for (int i = 0; i < expectedPath.size(); i++)
    {
        auto expected = expectedPath.front();
        auto actual = result.Path.front();

        EXPECT_EQ(expected, actual);

        expectedPath.pop_front();
        result.Path.pop_front();
    }
}

TEST_F(NavigationTest, ConcaveLevel)
{
    NavRequest request;
    request.StartPosition = IntVector(3, 10, 0);
    request.TargetPosition = IntVector(20, 20, 0);
    request.Traverser = ConcaveLevel;

    NavResult result = Nav.TryCalculatePath(request);

    ASSERT_TRUE(result.Succeeded);
    ASSERT_EQ(result.Path.size(), 26);
}

TEST_F(NavigationTest, RoadLevel)
{
    NavRequest request;
    request.StartPosition = IntVector(0, 5, 0);
    request.TargetPosition = IntVector(15, 15, 0);
    request.Traverser = RoadLevel;

    std::list expectedPath = {
        IntVector(0, 5, 0),
        IntVector(1, 5, 0),
        IntVector(2, 5, 0),
        IntVector(3, 5, 0),
        IntVector(4, 5, 0),
        IntVector(5, 5, 0),
        IntVector(6, 5, 0),
        IntVector(7, 5, 0),
        IntVector(8, 5, 0),
        IntVector(9, 5, 0),
        IntVector(10, 5, 0),
        IntVector(11, 5, 0),
        IntVector(12, 5, 0),
        IntVector(13, 5, 0),
        IntVector(14, 5, 0),
        IntVector(15, 6, 0),
        IntVector(15, 7, 0),
        IntVector(15, 8, 0),
        IntVector(15, 9, 0),
        IntVector(15, 10, 0),
        IntVector(15, 11, 0),
        IntVector(15, 12, 0),
        IntVector(15, 13, 0),
        IntVector(15, 14, 0),
        IntVector(15, 15, 0),
    };

    NavResult result = Nav.TryCalculatePath(request);

    ASSERT_TRUE(result.Succeeded);
    ASSERT_EQ(result.Path.size(), expectedPath.size());
    for (int i = 0; i < expectedPath.size(); i++)
    {
        auto expected = expectedPath.front();
        auto actual = result.Path.front();

        EXPECT_EQ(expected, actual);

        expectedPath.pop_front();
        result.Path.pop_front();
    }
}

TEST_F(NavigationTest, PathOutOfBounds)
{
    NavRequest request;
    request.StartPosition = IntVector(0, 0, 0);
    request.TargetPosition = IntVector(-1, 0, 0);
    request.Traverser = FlatLevelNoDifficulty;

    NavResult result = Nav.TryCalculatePath(request);

    ASSERT_FALSE(result.Succeeded);
}

TEST_F(NavigationTest, PathInWall)
{
    NavRequest request;
    request.StartPosition = IntVector(0, 0, 0);
    request.TargetPosition = IntVector(5, 5, 0);
    request.Traverser = ConcaveLevel;

    NavResult result = Nav.TryCalculatePath(request);

    ASSERT_FALSE(result.Succeeded);
}