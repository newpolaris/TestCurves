#include <gtest/gtest.h>
#include "el_vec4.h"

using namespace el;

//------------------------------------------------------------------------------
// A macro to help with vector comparisons within floating point range.
#define EXPECT_VEC_EQ(VEC1, VEC2)                               \
do {                                                            \
    const decltype(VEC1) v1 = VEC1;                             \
    const decltype(VEC2) v2 = VEC2;                             \
    if (std::is_same<TypeParam,float>::value) {                 \
        for (size_t i = 0; i < v1.size(); ++i) {                   \
            EXPECT_FLOAT_EQ(v1[i], v2[i]);                      \
        }                                                       \
    } else if (std::is_same<TypeParam,double>::value) {         \
        for (size_t i = 0; i < v1.size(); ++i) {                   \
            EXPECT_DOUBLE_EQ(v1[i], v2[i]);                     \
        }                                                       \
    } else {                                                    \
        for (size_t i = 0; i < v1.size(); ++i) {                   \
            EXPECT_EQ(v1[i], v2[i]);                            \
        }                                                       \
    }                                                           \
} while(0)

class VecTest : public testing::Test {
protected:
};

TEST_F(VecTest, constexpr) {
    constexpr double4 v1{};
}

TEST_F(VecTest, Basics) {
    double4 v4;
#if EL_SUPPORT_UNRESTRICTED_UNIONS
    double3& v3(v4.xyz);

    EXPECT_EQ(sizeof(double4), sizeof(double)*4);
    EXPECT_EQ(sizeof(double3), sizeof(double)*3);
    EXPECT_EQ(sizeof(double2), sizeof(double)*2);
    EXPECT_EQ(reinterpret_cast<void*>(&v3), reinterpret_cast<void*>(&v4));
#endif // EL_SUPPORT_UNRESTRICTED_UNIONS
}

TEST_F(VecTest, Swizzle) {
    double4 v4;
#if EL_SUPPORT_UNRESTRICTED_UNIONS
    double3& xyz(v4.xyz);
    double2& xy(v4.xy);
    double2& yz(v4.yz);
    double2& zw(v4.zw);
    double& x(v4.x);
    double& y(v4.y);
    double& z(v4.z);
    double& w(v4.w);
#endif // EL_SUPPORT_UNRESTRICTED_UNIONS
}

TEST_F(VecTest, Arithmetic) {
    double4 a(1.0);
    double4 b(2.0);
    double4 c = a + b;
    EXPECT_EQ(c.r, 3);
    EXPECT_EQ(c.g, 3);
    EXPECT_EQ(c.b, 3);

    double4 d = a * b * c;
    EXPECT_EQ(d.r, 6);
    EXPECT_EQ(d.g, 6);
    EXPECT_EQ(d.b, 6);
}

float3 linear_bazier(float3 start_position, float3 end_position, float t)
{
    return mix(start_position, end_position, t);
}

float3 quadratic_bazier(float3 positions[3], float t)
{
    const float q = (1.f - t);
    return q * q * positions[0] + 2*t * q*positions[1] + t*t*positions[2];
}

namespace jump_option {
    float depth = 5.f;
    float alpha = 0.3f;
    float edge0 = 0.3f;
    float edge1 = 1.5f;

    constexpr float offsetZ = -1.2f;
    constexpr float offsetX = -2.2f;

    float3 pathPoints[] = {
        float3(0.f, 0.f, 0.f),
        float3(offsetX/2, 0.f, offsetZ),
        float3(offsetX, 0.f, 0.f)
    };
}

struct CameraData
{
    mat4f view;
    mat4f proj;
};

void test()
{
    auto getReflectMtx = []() {
        mat4f m(1.f);
        // 2. move to below -1.0 (-3, -1)
        m = m * mat4f::translation(float3(0.f, -2.f, 0.f));
        // 1. y-invert (-1, 1)
        m = m * mat4f::scaling(float3(1.f, -1.f, 1.f));
        return m;
    };
}


class MatTest : public testing::Test {
protected:
};

TEST_F(MatTest, Constructors) {
    mat4 m0;
    ASSERT_EQ(m0[0].x, 1);
    ASSERT_EQ(m0[0].y, 0);
    ASSERT_EQ(m0[0].z, 0);
    ASSERT_EQ(m0[0].w, 0);
    ASSERT_EQ(m0[1].x, 0);
    ASSERT_EQ(m0[1].y, 1);
    ASSERT_EQ(m0[1].z, 0);
    ASSERT_EQ(m0[1].w, 0);
    ASSERT_EQ(m0[2].x, 0);
    ASSERT_EQ(m0[2].y, 0);
    ASSERT_EQ(m0[2].z, 1);
    ASSERT_EQ(m0[2].w, 0);
    ASSERT_EQ(m0[3].x, 0);
    ASSERT_EQ(m0[3].y, 0);
    ASSERT_EQ(m0[3].z, 0);
    ASSERT_EQ(m0[3].w, 1);

    mat4 m1(2);
}

TEST_F(MatTest, Arithmetic) {
    mat4f m(1.f);
    mat4f m2 = m * m;
    EXPECT_EQ(m2[0][0], 1.f);
    EXPECT_EQ(m2[1][1], 1.f);
    EXPECT_EQ(m2[2][2], 1.f);
    EXPECT_EQ(m2[3][3], 1.f);
    EXPECT_EQ(m2[0][1], 0.f);
}

template <typename T>
class MatTestT : public ::testing::Test {
public:
};

typedef ::testing::Types<float, double> TestMatrixValueTypes;

TYPED_TEST_SUITE(MatTestT, TestMatrixValueTypes);

TYPED_TEST(MatTestT, Translation4) {
    typedef el::details::TMat44<TypeParam> M44T;
    typedef el::details::TVec4<TypeParam> V4T;
    typedef el::details::TVec3<TypeParam> V3T;

    V3T translateBy(-7.3, 1.1, 14.4);
    V3T translation(translateBy[0], translateBy[1], translateBy[2]);
    M44T translation_matrix = M44T::translation(translation);

    V4T p1(9.9, 3.1, 41.1, 1.0);
    V4T p2(-18.0, 0.0, 1.77, 1.0);
    V4T p3(0, 0, 0, 1);
    V4T p4(-1000, -1000, 1000, 1.0);

    EXPECT_VEC_EQ((translation_matrix * p1).xyz, translateBy + p1.xyz);
    EXPECT_VEC_EQ((translation_matrix * p2).xyz, translateBy + p2.xyz);
    EXPECT_VEC_EQ((translation_matrix * p3).xyz, translateBy + p3.xyz);
    EXPECT_VEC_EQ((translation_matrix * p4).xyz, translateBy + p4.xyz);

    translation_matrix = M44T::translation(V3T{2.7});
    EXPECT_VEC_EQ((translation_matrix * p1).xyz, V3T{2.7} + p1.xyz);

    double k = clamp(1.0, 0.5, 3.5);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}