#ifndef __EL_MATH_TYPES_H__
#define __EL_MATH_TYPES_H__

#include <cassert>

// https://docs.microsoft.com/en-us/previous-versions/hh567368(v=vs.140)
// Unrestricted unions (since 2015)
#define SUPPORT_UNRESTRICTED_UNIONS 0
#if _MSC_VER >= 1900
#undef SUPPORT_UNRESTRICTED_UNIONS
#define SUPPORT_UNRESTRICTED_UNIONS 1
#endif

namespace el {
namespace math {
    
// https://stackoverflow.com/questions/12701469/why-is-the-empty-base-class-optimization-ebo-is-not-working-in-msvc
// MSVS does not support EBO for multiple inheritance
template <template <typename T> class VECTOR, typename T>
struct VecOperators
{
    VECTOR<T>& operator+=(const VECTOR<T>& v)
    {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++)
            lhs[i] += v[i];
        return lhs;
    }

    template <typename OHTER>
    VECTOR<T>& operator+=(const VECTOR<OHTER>& v)
    {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++)
            lhs[i] += v[i];
        return lhs;
    }

    friend inline VECTOR<T> operator+(VECTOR<T> lv, const VECTOR<T>& rv)
    {
        return lv += rv;
    }

    template <typename U>
    friend inline VECTOR<T> operator+(VECTOR<T> lv, const VECTOR<U>& rv)
    {
        return lv += rv;
    }

    VECTOR<T>& operator*=(const VECTOR<T>& v)
    {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++)
            lhs[i] *= v[i];
        return lhs;
    }

    template <typename OHTER>
    VECTOR<T>& operator*=(const VECTOR<OHTER>& v)
    {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++)
            lhs[i] *= v[i];
        return lhs;
    }

    friend inline VECTOR<T> operator*(VECTOR<T> lv, const VECTOR<T>& rv)
    {
        return lv *= rv;
    }

    template <typename U>
    friend inline VECTOR<T> operator*(VECTOR<T> lv, const VECTOR<U>& rv)
    {
        return lv *= rv;
    }

    VECTOR<T>& operator/=(const VECTOR<T>& v)
    {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++)
            lhs[i] /= v[i];
        return lhs;
    }

    template <typename OHTER>
    VECTOR<T>& operator/=(const VECTOR<OHTER>& v)
    {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++)
            lhs[i] /= v[i];
        return lhs;
    }

    friend inline VECTOR<T> operator/(VECTOR<T> lv, const VECTOR<T>& rv)
    {
        return lv /= rv;
    }

    template <typename U>
    friend inline VECTOR<T> operator/(VECTOR<T> lv, const VECTOR<U>& rv)
    {
        return lv /= rv;
    }

    friend inline bool operator==(const VECTOR<T>& lv, const VECTOR<T>& rv)
    {
        for (size_t i = 0; i < lv.size(); i++)
            if (lv[i] != rv[i])
                return false;
        return true;
    }

    friend inline bool operator!=(const VECTOR<T>& lv, const VECTOR<T>& rv)
    {
        return !operator==(lv, rv);
    }
};

template <typename T> struct TVec2;
template <typename T> struct TVec3;
template <typename T> struct TVec4;

template <typename T>
struct TVec2 : public VecOperators<TVec2, T>
{
    typedef T value_type;
    typedef T& referrence;
    typedef T const& const_reference;
    typedef size_t size_type;
    static const size_t SIZE = 2;

    TVec2() {}
    
    template <typename A>
    TVec2(A v) : x(v), y(v) {} 

    template <typename A, typename B>
    TVec2(A x, B y) : x(x), y(y) {}

    template <typename A>
    TVec2(const TVec2<A>& v) : x(v.x), y(v.y) {}

    template <typename A>
    TVec2(const TVec3<A>& v) : x(v.x), y(v.y) {}

    template <typename A>
    TVec2(const TVec4<A>& v) : x(v.x), y(v.y) {}

    size_t size() const {
        return SIZE;
    }

    const T& operator[](size_t i) const {
        assert(i < SIZE);
        return v[i];
    }

    T& operator[](size_t i) {
        assert(i < SIZE);
        return v[i];
    }

    union {
        T v[SIZE];
        struct { T x, y; };
        struct { T r, g; };
        struct { T s, t; };
    };
};

template <typename T>
struct TVec3 : public VecOperators<TVec3, T>
{
    typedef T value_type;
    typedef T& referrence;
    typedef T const& const_reference;
    typedef size_t size_type;
    static const size_t SIZE = 3;

    TVec3() {}
    
    template <typename A>
    TVec3(A v) : x(v), y(v), z(v) {} 

    template <typename A, typename B, typename C>
    TVec3(A x, B y, C z) : x(x), y(y), z(z) {}

    template <typename A>
    TVec3(const TVec3<A>& v) : x(v.x), y(v.y), z(v.z) {}

    template <typename A>
    TVec3(const TVec4<A>& v) : x(v.x), y(v.y), z(v.z) {}

    template <typename A, typename B, typename C>
    TVec3(const TVec2<A>& a, B b) :
        x(static_cast<T>(a.x)),
        y(static_cast<T>(a.y)),
        z(static_cast<T>(b)) {}

    size_t size() const {
        return SIZE;
    }

    const T& operator[](size_t i) const {
        assert(i < SIZE);
        return v[i];
    }

    T& operator[](size_t i) {
        assert(i < SIZE);
        return v[i];
    }

    union {
        T v[SIZE];
#if SUPPORT_UNRESTRICTED_UNIONS
        TVec2<T> xy, rg;
        struct {
            union { T x, r; };
            union {
                TVec2<T> yz, gb;
                struct { T y, z; };
                struct { T g, b; };
            };
        };
#else
        struct { T x, y, z; };
		struct { T r, g, b; };
#endif
    };
};

template <typename T>
struct TVec4 : public VecOperators<TVec4, T>
{
    typedef T value_type;
    typedef T& referrence;
    typedef T const& const_reference;
    typedef size_t size_type;
    static const size_t SIZE = 4;

    TVec4() {}
    
    template <typename A>
    TVec4(A v) : x(v), y(v), z(v), w(v) {} 

    template <typename A, typename B, typename C, typename D>
    TVec4(A x, B y, C z, D w) : 
        x(static_cast<T>(x)),
        y(static_cast<T>(y)),
        z(static_cast<T>(z)),
        w(static_cast<T>(w)) {}

    template <typename A, typename B, typename C>
    TVec4(const TVec2<A>& a, B b, C c) :
        x(static_cast<T>(a.x)),
        y(static_cast<T>(a.y)),
        z(static_cast<T>(b)),
        w(static_cast<T>(c)) {}

    template <typename A, typename B, typename C>
    TVec4(const TVec3<A>& a, B b) :
        x(static_cast<T>(a.x)),
        y(static_cast<T>(a.y)),
        z(static_cast<T>(a.z)),
        w(static_cast<T>(b)) {}


    size_t size() const {
        return SIZE;
    }

    const T& operator[](size_t i) const {
        assert(i < SIZE);
        return v[i];
    }

    T& operator[](size_t i) {
        assert(i < SIZE);
        return v[i];
    }

    union {
        T v[SIZE];
#if SUPPORT_UNRESTRICTED_UNIONS
        TVec2<T> xy, rg;
        TVec3<T> xyz, rgb;
        struct {
            union { T x, r; };
            union {
                TVec2<T> yz, gb;
                TVec3<T> yzw, gba;
                struct {
                    union { T y, g; };
                    union {
                        TVec2<T> zw, ba;
                        struct { T z, w; };
                        struct { T b, a; };
                    };
                };
            };
        };
#else
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
#endif
    };
};

typedef TVec2<float> float2;
typedef TVec3<float> float3;
typedef TVec4<float> float4;

static_assert(sizeof(float2) == sizeof(float) * 2, "ebo not supported");
static_assert(sizeof(float3) == sizeof(float) * 3, "ebo not supported");
static_assert(sizeof(float4) == sizeof(float) * 4, "ebo not supported");

typedef TVec2<uint8_t> ucvec2;
typedef TVec3<uint8_t> ucvec3;
typedef TVec4<uint8_t> ucvec4;

typedef TVec2<uint32_t> uvec2;
typedef TVec3<uint32_t> uvec3;
typedef TVec4<uint32_t> uvec4;

typedef TVec2<int32_t> ivec2;
typedef TVec3<int32_t> ivec3;
typedef TVec4<int32_t> ivec4;

typedef float scalar;
typedef TVec2<scalar> vec2;
typedef TVec3<scalar> vec3;
typedef TVec4<scalar> vec4;

enum EnumZero { ZERO };
enum EnumIdentity { IDENTITY };

template <typename T>
struct TMat4x4
{
    typedef T value_type;
    typedef T& referrence;
    typedef T const& const_reference;
    typedef size_t size_type;

    typedef TVec4<value_type> row_type;
    typedef TVec4<value_type> col_type;

    static const size_type ROW_SIZE = row_type::SIZE;
    static const size_type COL_SIZE = col_type::SIZE;
    static const size_type NUM_ROWS = ROW_SIZE;
    static const size_type NUM_COLS = COL_SIZE;

    col_type _data[NUM_COLS];

    TMat4x4();
    explicit TMat4x4(EnumZero);
    explicit TMat4x4(EnumIdentity);

    template <typename A>
    explicit TMat4x4(A v);

    template <
        typename A, typename B, typename C, typename D,
        typename E, typename F, typename G, typename H,
        typename I, typename J, typename K, typename L,
        typename M, typename N, typename O, typename P>
    explicit TMat4x4(A m00, B m01, C m02, D m03,
                    E m10, F m11, G m12, H m13,
                    I m20, J m21, K m22, L m23,
                    M m30, N m31, O m32, P m33);

    const col_type& operator[](size_t col) const
    {
        assert(col < NUM_COLS);
        return _data[col];
    }

    col_type& operator[](size_t col)
    {
        assert(col < NUM_COLS);
        return _data[col];
    }
};

template <typename T>
TMat4x4<T>::TMat4x4()
{
}

template <typename T>
TMat4x4<T>::TMat4x4(EnumZero)
{
    for (int i = 0; i < NUM_COLS; i++)
        _data[i] = col_type(0, 0, 0, 0);
}

template <typename T>
TMat4x4<T>::TMat4x4(EnumIdentity)
{
    _data[0] = col_type(1, 0, 0, 0);
    _data[1] = col_type(0, 1, 0, 0);
    _data[2] = col_type(0, 0, 1, 0);
    _data[3] = col_type(0, 0, 0, 1);
}

template <typename T>
template <typename A>
TMat4x4<T>::TMat4x4(A v)
{
    for (int i = 0; i < NUM_COLS; i++)
        _data[i] = col_type(v, v, v, v);
}

template <typename T>
template <
    typename A, typename B, typename C, typename D,
    typename E, typename F, typename G, typename H,
    typename I, typename J, typename K, typename L,
    typename M, typename N, typename O, typename P>
TMat4x4<T>::TMat4x4(A m00, B m01, C m02, D m03,
                    E m10, F m11, G m12, H m13,
                    I m20, J m21, K m22, L m23,
                    M m30, N m31, O m32, P m33)
{
    _data[0] = col_type(m00, m01, m02, m03);
    _data[1] = col_type(m10, m11, m12, m13);
    _data[2] = col_type(m20, m21, m22, m23);
    _data[3] = col_type(m30, m31, m32, m33);
}

typedef TMat4x4<float> mat4;

} // namespace math
} // namespace el

#undef SUPPORT_UNRESTRICTED_UNIONS

#endif //__EL_MATH_TYPES_H__
