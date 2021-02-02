#pragma once

#define VEC3_EPSILON 0.000001f

namespace gszauer {

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

} // namespace gszauer
