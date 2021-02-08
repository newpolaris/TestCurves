#include "Vec3.h"
#include <cmath>

namespace gszauer {

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

vec3 slerp(const vec3& s, const vec3& e, float t) {
    vec3 from = normalized(s);
    vec3 to = normalized(e);
    float theta = angle(from, to);
    float sin_theta = sinf(theta);
    float a = sinf((1.0f - t) * theta) / sin_theta;
    float b = sinf(t * theta) / sin_theta;

    return from * a + to * b;
}

vec3 nlerp(const vec3& s, const vec3& e, float t) {
    return normalized(lerp(s, e, t));
}

float dot(const vec3& l, const vec3& r)
{
    return l.x * r.x + l.y * r.y + l.z * r.z;
}

float lenSq(const vec3& v) {
    return dot(v, v);
}

float len(const vec3& v) {
    float sq = lenSq(v);
    if (sq < VEC3_EPSILON) {
        return 0.0f;
    }
    return sqrtf(sq);
}

vec3 normalized(const vec3& v) {
    float invLen = 1.0f / len(v);
    return v * invLen;
}

void normalize(vec3& v) {
    v = normalized(v);
}

float angle(const vec3& l, const vec3& r)
{
    float lenL = len(l);
    float lenR = len(r);
    if (lenL < VEC3_EPSILON || lenR < VEC3_EPSILON)
        return 0.f;
    float o = dot(l, r);
    return acosf(o / lenL / lenR);
}

vec3 project(const vec3& a, const vec3& b)
{
    float magBSq = lenSq(b);
    if (magBSq < VEC3_EPSILON) {
        return vec3();
    }
    float scale = dot(a, b) / magBSq;
    return b * scale;
}

vec3 reject(const vec3& a, const vec3& b) {
    vec3 projection = project(a, b);
    return a - projection;
}

vec3 reflect(const vec3& a, const vec3& b) {
    float magBSq = lenSq(b);
    if (magBSq < VEC3_EPSILON) {
        return vec3();
    }
    float scale = dot(a, b) / magBSq;
    vec3 proj2 = b * (scale * 2);
    return a - proj2;
}

vec3 cross(const vec3& l, const vec3& r) {
    return vec3(
        l.y * r.z - l.z * r.y,
        l.z * r.x - l.x * r.z,
        l.x * r.y - l.y * r.x);
}

bool operator==(const vec3& l, const vec3& r)
{
    vec3 diff(l - r);
    return lenSq(diff) < VEC3_EPSILON;
}

bool operator!=(const vec3& l, const vec3& r)
{
    return !(l == r);
}

} // namespace gszauer
