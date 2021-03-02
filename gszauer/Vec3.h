#pragma once

#include <cassert>
#include <cstdint>
#include <utility>

#pragma warning(disable : 4067 4068)

#if __has_attribute(pure)
#   define MATH_PURE __attribute__((pure))
#else
#   define MATH_PURE
#endif

#define VEC3_EPSILON 0.000001f

namespace gszauer {

template <typename T> struct TVec2;
template <typename T> struct TVec3;
template <typename T> struct TVec4;

template <typename T>
struct TVec2 {
    union {
        struct {
            float x;
            float y;
        };
        T v[2]{};
    };

    constexpr TVec2() : x(T(0)), y(T(0)) {}
    TVec2(T x) : x(x), y(x) {}
    TVec2(T x, T y) : x(x), y(y) {}
    TVec2(T* fv) : x(fv[0]), y(fv[1]) {}
    TVec2(const TVec3<T>& vec) 
        : v { vec[0], vec[1] }
    {}
};
        
template <typename T> 
constexpr inline TVec2<T> operator+(const TVec2<T>& l, const TVec2<T>& r)
{
    return TVec2<T>(l.x + r.x, l.y + r.y);
}

template <typename T>
constexpr inline TVec2<T> operator-(const TVec2<T>& l, const TVec2<T>& r)
{
    return TVec2<T>{l.x - r.x, l.y - r.y};
}

template <typename T>
constexpr inline TVec2<T> operator*(const TVec2<T>& l, const TVec2<T>& r)
{
    return TVec2<T>{l.x * r.x, l.y * r.y};
}

template <typename T>
constexpr inline TVec2<T> operator/(const TVec2<T>& l, const TVec2<T>& r)
{
    return TVec2<T>{l.x / r.x, l.y / r.y};
}

template <typename T> 
struct TVec3
{
    static constexpr size_t SIZE = 3;
    union {
        T v[SIZE]{};
        struct {
            T x, y, z;
        };
    };
    constexpr TVec3() : v{ T(0.0), T(0.0), T(0.0) } {}
    TVec3(T x, T y, T z) :
        x(x), y(y), z(z) {}
    TVec3(T * fv) :
        x(fv[0]), y(fv[1]), z(fv[2]) {}

    inline constexpr T& operator[](size_t i) noexcept {
        assert(i < SIZE);
        return v[i];
    }

    inline constexpr const T& operator[](size_t i) const noexcept {
        assert(i < SIZE);
        return v[i];
    }

    constexpr size_t size() const { return SIZE; }
};

template <typename T>
TVec3<T> operator+(const TVec3<T>& l, const TVec3<T>& r)
{
    return TVec3<T>(l.x + r.x, l.y + r.y, l.z + r.z);
}

template <typename T>
TVec3<T> operator-(const TVec3<T>& l, const TVec3<T>& r)
{
    return TVec3<T>(l.x - r.x, l.y - r.y, l.z - r.z);
}

template <typename T>
TVec3<T> operator*(const TVec3<T>& l, float f)
{
    return TVec3<T>(l.x * f, l.y * f, l.z * f);
}

template <typename T>
TVec3<T> operator*(const TVec3<T>& l, const TVec3<T>& r)
{
    return TVec3<T>(l.x * r.x, l.y * r.y, l.z * r.z);
}

template <typename T>
TVec3<T> operator/(const TVec3<T>& l, const TVec3<T>& r)
{
    return TVec3<T>(l.x / r.x, l.y / r.x, l.z / r.x);
}

template <typename T>
TVec3<T> operator/(const TVec3<T>& l, float f)
{
    return TVec3<T>(l.x / f, l.y / f, l.z / f);
}

template <typename T>
TVec3<T> operator*(float f, const TVec3<T>& l)
{
    return l * f;
}

template <typename T>
TVec3<T> lerp(const TVec3<T>& s, const TVec3<T>& e, float t)
{
    return e * t + s * (1 - t);
}

template <typename T>
TVec3<T> slerp(const TVec3<T>& s, const TVec3<T>& e, float t) {
    TVec3<T> from = normalized(s);
    TVec3<T> to = normalized(e);
    float theta = angle(from, to);
    float sin_theta = sinf(theta);
    float a = sinf((1.0f - t) * theta) / sin_theta;
    float b = sinf(t * theta) / sin_theta;

    return from * a + to * b;
}

template <typename T>
TVec3<T> nlerp(const TVec3<T>& s, const TVec3<T>& e, float t) {
    return normalized(lerp(s, e, t));
}

template <typename T>
T dot(const TVec3<T>& l, const TVec3<T>& r)
{
    return l.x * r.x + l.y * r.y + l.z * r.z;
}

template <typename T>
T lenSq(const TVec3<T>& v) {
    return dot(v, v);
}

template <typename T>
T len(const TVec3<T>& v) {
    float sq = lenSq(v);
    if (sq < VEC3_EPSILON) {
        return 0.0f;
    }
    return sqrtf(sq);
}

template <typename T>
TVec3<T> normalized(const TVec3<T>& v) {
    T invLen = (T)1.0 / len(v);
    return v * invLen;
}

template <typename T>
void normalize(TVec3<T>& v) {
    v = normalized(v);
}

template <typename T>
T angle(const TVec3<T>& l, const TVec3<T>& r)
{
    T lenL = len(l);
    T lenR = len(r);
    if (lenL < VEC3_EPSILON || lenR < VEC3_EPSILON)
        return 0.f;
    T o = dot(l, r);
    return acosf(o / lenL / lenR);
}

template <typename T>
TVec3<T> project(const TVec3<T>& a, const TVec3<T>& b)
{
    float magBSq = lenSq(b);
    if (magBSq < VEC3_EPSILON) {
        return TVec3<T>();
    }
    float scale = dot(a, b) / magBSq;
    return b * scale;
}

template <typename T>
TVec3<T> reject(const TVec3<T>& a, const TVec3<T>& b) {
    TVec3<T> projection = project(a, b);
    return a - projection;
}

template <typename T>
TVec3<T> reflect(const TVec3<T>& a, const TVec3<T>& b) {
    float magBSq = lenSq(b);
    if (magBSq < VEC3_EPSILON) {
        return TVec3<T>();
    }
    float scale = dot(a, b) / magBSq;
    TVec3<T> proj2 = b * (scale * 2);
    return a - proj2;
}

template <typename T>
TVec3<T> cross(const TVec3<T>& l, const TVec3<T>& r) {
    return TVec3<T>(
        l.y * r.z - l.z * r.y,
        l.z * r.x - l.x * r.z,
        l.x * r.y - l.y * r.x);
}

template <typename T>
bool operator==(const TVec3<T>& l, const TVec3<T>& r)
{
    TVec3<T> diff(l - r);
    return lenSq(diff) < VEC3_EPSILON;
}

template <typename T>
bool operator!=(const TVec3<T>& l, const TVec3<T>& r)
{
    return !(l == r);
}

template <typename T>
struct TVec4 {

    static constexpr size_t SIZE = 4;

    union {
        T v[SIZE]{};
        TVec2<T> xy, st, rg;
        TVec3<T> xyz, stp, rgb;
        struct {
            union { T x, s, r; };
            union {
                TVec2<T> yz;
                TVec3<T> yzw;
                struct {
                    union { T y, t, g; };
                    union {
                        TVec2<T> zw, pq, ba;
                        struct { T z, w; };
                        struct { T p, q; };
                        struct { T b, a; };
                    };
                };
            };
        };
    };

    constexpr TVec4() : x(0.0), y(0.0), z(0.0), w(0.0) {}
    template <typename A>
    constexpr TVec4(A x) 
        : v{ T(x), T(x), T(x), T(x) } {}
    template <typename A, typename B, typename C, typename D>
    constexpr TVec4(A x, B y, C z, D w) 
        : x(T(x)), y(T(y)), z(T(z)), w(T(w)) {}
    template <typename A>
    constexpr TVec4(A* fv) 
        : x(T(fv[0])), y(T(fv[1])), z(T(fv[2])), w(T(fv[3])) {}

    inline constexpr const T& operator[](size_t i) const noexcept {
        assert(i < SIZE);
        return v[i];
    }

    inline constexpr T& operator[](size_t i) noexcept {
        assert(i < SIZE);
        return v[i];
    }

    template <typename U>
    constexpr TVec4<T>& operator+=(const TVec4<U>& u) {
        TVec4<T>& lhs = *this;
        for (size_t i = 0; i < 4; i++)
            lhs[i] += u[i];
        return lhs;
    }

    template <typename U>
    constexpr TVec4<T>& operator-=(const TVec4<U>& u) {
        TVec4<T>& lhs = *this;
        for (size_t i = 0; i < 4; i++)
            lhs[i] -= u[i];
        return lhs;
    }

    template <typename U>
    constexpr TVec4<T>& operator*=(const TVec4<U>& u) {
        TVec4<T>& lhs = *this;
        for (size_t i = 0; i < 4; i++)
            lhs[i] *= u[i];
        return lhs;
    }
};

template <typename T>
inline constexpr 
TVec4<T> operator+(const TVec4<T>& l, const TVec4<T>& r)
{
    TVec4<T> res(l);
    res += r;
    return res;
}

template <typename T>
inline constexpr 
TVec4<T> operator-(const TVec4<T>& l, const TVec4<T>& r)
{
    TVec4<T> res(l);
    res -= r;
    return res;
}

template <typename T>
inline constexpr 
TVec4<T> operator*(const TVec4<T>& l, const TVec4<T>& r)
{
    TVec4<T> res(l);
    res *= r;
    return res;
}

template <typename T>
class Bezier {
public:
    T P1; // Point 1
    T C1; // Control 1
    T P2; // Point 2
    T C2; // Control 2
};

template <typename T>
inline T interpolate(const Bezier<T>& curve, float t) {
    T A = lerp(curve.P1, curve.C1, t);
    T B = lerp(curve.C2, curve.P2, t);
    T C = lerp(curve.C1, curve.C2, t);
    T D = lerp(A, C, t);
    T E = lerp(C, B, t);
    T R = lerp(D, E, t);
    return R;
}

} // namespace gszauer

template<typename T> using vector2 = gszauer::TVec2<T>;
template<typename T> using vector3 = gszauer::TVec3<T>;
template<typename T> using vector4 = gszauer::TVec4<T>;

using ivec2 = vector2<int>;
using vec2 = vector2<float>;
using float2 = vector2<float>;
using double2 = vector2<double>;
using ivec3 = vector3<int>;
using vec3 = vector3<float>;
using float3 = vector3<float>;
using double3 = vector3<double>;
using ivec4 = vector4<int>;
using vec4 = vector4<float>;
using float4 = vector4<float>;
using double4 = vector4<double>;
