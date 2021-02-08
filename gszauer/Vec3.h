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

using vec2 = TVec2<float>;
using ivec2 = TVec2<int>;
using vec4 = TVec4<float>;
using ivec4 = TVec4<int>;

struct vec3;

template <typename T>
struct TVec2 {
    union {
        struct {
            float x;
            float y;
        };
        float v[2];
    };
    TVec2() : x(T(0)), y(T(0)) {}
    TVec2(T x) : x(x), y(x) {}
    TVec2(T x, T y) : x(x), y(y) {}
    TVec2(T* fv) : x(fv[0]), y(fv[1]) {}
    TVec2(const vec3& vec) 
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

struct vec3 
{
    static constexpr size_t SIZE = 3;

    union {
        struct {
            float x;
            float y;
            float z;
        };
        float v[SIZE];
    };
    vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    vec3(float x, float y, float z) :
        x(x), y(y), z(z) {}
    vec3(float* fv) :
        x(fv[0]), y(fv[1]), z(fv[2]) {}

    inline constexpr float& operator[](size_t i) noexcept {
        assert(i < SIZE);
        return v[i];
    }

    inline constexpr const float& operator[](size_t i) const noexcept {
        assert(i < SIZE);
        return v[i];
    }

    constexpr size_t size() const { return SIZE; }
};

vec3 operator+(const vec3& l, const vec3& r);
vec3 operator-(const vec3& l, const vec3& r);
vec3 operator*(const vec3& l, float f);
vec3 operator*(const vec3& l, const vec3& r);
vec3 operator/(const vec3& l, const vec3& r);
vec3 operator/(const vec3& l, float f);
vec3 operator*(float f, const vec3& l);

float dot(const vec3& l, const vec3& r);
float lenSq(const vec3& v);
float len(const vec3& v);
vec3 normalized(const vec3& v);
void normalize(vec3& v);

float angle(const vec3& l, const vec3& r);

vec3 project(const vec3& a, const vec3& b);
vec3 reject(const vec3& a, const vec3& b);
vec3 reflect(const vec3& a, const vec3& b);

vec3 cross(const vec3& l, const vec3& r);

vec3 lerp(const vec3& s, const vec3& e, float t);
vec3 slerp(const vec3& s, const vec3& e, float t);
vec3 nlerp(const vec3& s, const vec3& e, float t);

bool operator==(const vec3& l, const vec3& r);
bool operator!=(const vec3& l, const vec3& r);

template <typename T>
struct TVec4 {

    static constexpr size_t SIZE = 4;

    union {
        struct {
            float x, y, z, w;
        };
        struct {
            float r, g, b, a;
        };
        float v[4];
    };
    constexpr TVec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    constexpr TVec4(float x, float y, float z, float w) 
        : x(x), y(y), z(z), w(w) {}
    constexpr TVec4(float* fv) 
        : x(fv[0]), y(fv[1]), z(fv[2]), w(fv[3]) {}

    inline constexpr const float& operator[](size_t i) const noexcept {
        assert(i < SIZE);
        return v[i];
    }

    inline constexpr float& operator[](size_t i) noexcept {
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
