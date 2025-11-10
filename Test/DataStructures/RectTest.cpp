#include "DataStructrues/HashedString.h"
#include "gtest/gtest.h"
#include "Primitives/Rect.h"

class RectTest : public testing::Test
{
protected:
    RectTest()
    {
        A = Rect(0, 0, 1, 1);
        B = Rect(-1, -1, 1, 1);
        C = Rect(-2, -2, 1, 1);
        D = Rect(-1, -1, 3, 3);
        E = Rect(0, 0, 10, 10);
        F = Rect(2, 2, 3, 3);
    }

    Rect A;
    Rect B;
    Rect C;
    Rect D;
    Rect E;
    Rect F;
};

TEST_F(RectTest, Intersects)
{
    std::vector<std::pair<std::pair<Rect, Rect>, bool>> Pairs
    {
        { { A, B }, true },
        { { A, C }, false },
        { { A, D }, true },
        { { A, E }, true },
        { { A, F }, false },
        { { E, F }, true }
    };

    for (const auto & KVP : Pairs)
    {
        auto pair = KVP.first;
        ASSERT_EQ(pair.first.Intersects(pair.second), KVP.second) << "Expected " << pair.first << ".Intersects" << pair.second << " = " << KVP.second;
        ASSERT_EQ(pair.second.Intersects(pair.first), KVP.second) << "Expected " << pair.second << ".Intersects" << pair.first << " = " << KVP.second;
    }
}