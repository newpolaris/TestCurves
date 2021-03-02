#include <math.h>
#include <gtest/gtest.h>

#include "gszauer/Vec3.h"

class VecTest : public testing::Test {
protected:
};

TEST_F(VecTest, Constexpr) {
}

TEST_F(VecTest, Basics) {
    double4 v4;

    double3& v3(v4.xyz);

    EXPECT_EQ(sizeof(double4), sizeof(double) * 4);
    EXPECT_EQ(sizeof(double3), sizeof(double) * 3);
    EXPECT_EQ(sizeof(double2), sizeof(double) * 2);
    EXPECT_EQ(reinterpret_cast<void*>(&v3), reinterpret_cast<void*>(&v4));
}

TEST_F(VecTest, Constructors) {
    double4 v1(1);
    EXPECT_EQ(v1.x, 1);
    EXPECT_EQ(v1.y, 1);
    EXPECT_EQ(v1.z, 1);
    EXPECT_EQ(v1.w, 1);

    double4 v2(1, 2, 3, 4);
    EXPECT_EQ(v2.x, 1);
    EXPECT_EQ(v2.y, 2);
    EXPECT_EQ(v2.z, 3);
    EXPECT_EQ(v2.w, 4);

    double4 v3(v2);
    EXPECT_EQ(v3.x, 1);
    EXPECT_EQ(v3.y, 2);
    EXPECT_EQ(v3.z, 3);
    EXPECT_EQ(v3.w, 4);

#if 0
    double4 v4(v3.xyz, 42);
    EXPECT_EQ(v4.x, 1);
    EXPECT_EQ(v4.y, 2);
    EXPECT_EQ(v4.z, 3);
    EXPECT_EQ(v4.w, 42);

    double4 v5(double3(v2.xy, 42), 24);
    EXPECT_EQ(v5.x, 1);
    EXPECT_EQ(v5.y, 2);
    EXPECT_EQ(v5.z, 42);
    EXPECT_EQ(v5.w, 24);
#endif

    float4 vf(2);
    EXPECT_EQ(vf.x, 2);
    EXPECT_EQ(vf.y, 2);
    EXPECT_EQ(vf.z, 2);
    EXPECT_EQ(vf.w, 2);
}

TEST_F(VecTest, ArithmeticOps) {
    double4 v0(1, 2, 3, 4);
    double4 v1(10, 20, 30, 40);

    double4 v2(v0 + v1);
    EXPECT_EQ(v2.x, 11);
    EXPECT_EQ(v2.y, 22);
    EXPECT_EQ(v2.z, 33);
    EXPECT_EQ(v2.w, 44);

#if 0
    v0 = v1 * 2;
    EXPECT_EQ(v0.x, 20);
    EXPECT_EQ(v0.y, 40);
    EXPECT_EQ(v0.z, 60);
    EXPECT_EQ(v0.w, 80);

    v0 = 2 * v1;
    EXPECT_EQ(v0.x, 20);
    EXPECT_EQ(v0.y, 40);
    EXPECT_EQ(v0.z, 60);
    EXPECT_EQ(v0.w, 80);

    float4 vf(2);
    v0 = v1 * vf;
    EXPECT_EQ(v0.x, 20);
    EXPECT_EQ(v0.y, 40);
    EXPECT_EQ(v0.z, 60);
    EXPECT_EQ(v0.w, 80);
#endif
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
