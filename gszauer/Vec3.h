#pragma once

#define VEC3_EPSILON 0.000001f

namespace gszauer {

struct vec2;
struct vec3;

struct vec2 {
    union {
        struct {
            float x;
            float y;
        };
        float v[2];
    };
    inline vec2() : x(0.0f), y(0.0f) {}
    inline vec2(float x) :
        x(x), y(x) {}
    inline vec2(float x, float y) :
        x(x), y(y) {}
    inline vec2(float* fv) :
        x(fv[0]), y(fv[1]) {}

    vec2(const vec3& vec);
};

vec2 operator+(const vec2& l, const vec2& r);
vec2 operator-(const vec2& l, const vec2& r);
vec2 operator*(const vec2& l, const vec2& r);
vec2 operator/(const vec2& l, const vec2& r);

struct vec3 {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float v[3];
    };
    inline vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    inline vec3(float x, float y, float z) :
        x(x), y(y), z(z) {}
    inline vec3(float* fv) :
        x(fv[0]), y(fv[1]), z(fv[2]) {}
};

vec3 operator+(const vec3& l, const vec3& r);
vec3 operator-(const vec3& l, const vec3& r);
vec3 operator*(const vec3& l, float f);
vec3 operator*(const vec3& l, const vec3& r);
vec3 operator/(const vec3& l, const vec3& r);
vec3 operator/(const vec3& l, float f);
vec3 operator*(float f, const vec3& l);

vec3 lerp(const vec3& s, const vec3& e, float t);

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

struct vec4 {
    union {
        struct {
            float x, y, z, w;
        };
        struct {
            float r, g, b, a;
        };
        float v[4];
    };
    inline vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    inline vec4(float x, float y, float z, float w) :
        x(x), y(y), z(z), w(w) {}
    inline vec4(float* fv) :
        x(fv[0]), y(fv[1]), z(fv[2]), w(fv[3]) {}
};

} // namespace gszauer
