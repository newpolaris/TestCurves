#include <math.h>
#include <gtest/gtest.h>

#include "gszauer/Mat4.h"

class MatTest : public testing::Test {
};

TEST_F(MatTest, Basics) {
    EXPECT_EQ(sizeof(mat4), sizeof(float)*16);
}

TEST_F(MatTest, Constructors) {
    mat4 m0;
    ASSERT_EQ(m0[0].x, 1);

}

TEST_F(MatTest, ArithmeticOps) {
    mat4 m0;
    mat4 m1(2);
    mat4 m2(float4(2));

    m1 += m2;
    EXPECT_EQ(mat4(4), m1);

    m2 -= m1;
    EXPECT_EQ(mat4(-2), m2);

    m1 *= 2;
    EXPECT_EQ(mat4(8), m1);

#if 0
    m1 /= 2;
    EXPECT_EQ(mat4(4), m1);
#endif
}
