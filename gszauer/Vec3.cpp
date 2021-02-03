#include "Vec3.h"

namespace gszauer {

vec2::vec2(const vec3& vec) :
        x(vec.x), y(vec.y) {}

vec2 operator+(const vec2& l, const vec2& r)
{
    return vec2(l.x + r.x, l.y + r.y);
}

vec2 operator-(const vec2& l, const vec2& r)
{
    return vec2(l.x - r.x, l.y - r.y);
}

vec2 operator*(const vec2& l, const vec2& r)
{
    return vec2(l.x * r.x, l.y * r.y);
}

vec2 operator/(const vec2& l, const vec2& r)
{
    return vec2(l.x / r.x, l.y / r.y);
}

vec3 operator+(const vec3& l, const vec3& r)
{
    return vec3(l.x + r.x, l.y + r.y, l.z + r.z);
}

vec3 operator-(const vec3& l, const vec3& r)
{
    return vec3(l.x - r.x, l.y - r.y, l.z - r.z);
}

vec3 operator*(const vec3& l, float f)
{
    return vec3(l.x * f, l.y * f, l.z * f);
}

vec3 operator*(const vec3& l, const vec3& r)
{
    return vec3(l.x * r.x, l.y * r.y, l.z * r.z);
}

vec3 operator/(const vec3& l, const vec3& r)
{
    return vec3(l.x / r.x, l.y / r.x, l.z / r.x);
}

vec3 operator/(const vec3& l, float f)
{
    return vec3(l.x / f, l.y / f, l.z / f);
}

vec3 operator*(float f, const vec3& l)
{
    return l * f;
}

vec3 lerp(const vec3& s, const vec3& e, float t)
{
    return e * t + s * (1 - t);
}

} // namespace gszauer
