#include <math.h>
#include <gtest/gtest.h>

class VecTest : public testing::Test {
protected:
};

TEST_F(VecTest, Constexpr) {
}

TEST_F(VecTest, Basics) {
#if 0
    double4 v4;
    double3& v3(v4.xyz);

    EXPECT_EQ(sizeof(double4), sizeof(double)*4);
    EXPECT_EQ(sizeof(double3), sizeof(double)*3);
    EXPECT_EQ(sizeof(double2), sizeof(double)*2);
    EXPECT_EQ(reinterpret_cast<void*>(&v3), reinterpret_cast<void*>(&v4));
#endif
}

TEST_F(VecTest, Access) {
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
