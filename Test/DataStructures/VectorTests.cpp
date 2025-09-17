#include "DataStructrues/HashedString.h"
#include "DataStructrues/Vector.h"
#include "gtest/gtest.h"

TEST(IntVector2D, DefaultInitialization)
{
    IntVector2D V;
    EXPECT_EQ(V.X, 0);
    EXPECT_EQ(V.Y, 0);
}

TEST(IntVector2D, Equality)
{
    IntVector2D A(1, 1);
    IntVector2D B(2, 2);

    EXPECT_EQ(A, A);
    EXPECT_NE(A, B);
}

TEST(IntVector2D, Arithmetic)
{
    IntVector2D A(1, 1);
    IntVector2D B(2, 1);

    EXPECT_EQ(A + B, IntVector2D(3, 2));
    EXPECT_EQ(A - B, IntVector2D(-1, 0));
}