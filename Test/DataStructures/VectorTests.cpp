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

TEST(IntVector2D, Length)
{
    IntVector2D A(1, 1);
    A.Normalize();
    EXPECT_EQ(A.X, 1);
    EXPECT_EQ(A.Y, 1);
    
    IntVector2D B(2, 0);
    B.Normalize();
    EXPECT_EQ(B.X, 1);
    EXPECT_EQ(B.Y, 0);
    
    IntVector2D C(0, 0);
    C.Normalize();
    EXPECT_EQ(C.X, 0);
    EXPECT_EQ(C.Y, 0);    
}