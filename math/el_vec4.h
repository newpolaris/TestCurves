#ifndef __EL_MATH_VEC4_H__
#define __EL_MATH_VEC4_H__

#include <cassert>
#include <cstddef>
#include <type_traits>

#include "el_macros.h"

namespace el {

template <typename T>
inline T min(T a, T b) noexcept {
    return a < b ? a : b;
}

template <typename T>
inline T max(T a, T b) noexcept {
    return a > b ? a : b;
}

template <typename T>
inline T clamp(T v, T min, T max) noexcept {
    assert(min <= max);
    return T(el::min(max, (el::max(min, v))));
}

template <typename T>
inline T mix(T x, T y, T a) noexcept {
    return x * (T(1) - a) + y * a;
}

} // namespace el

namespace el {
namespace details {

template <typename T, typename U>
struct arithmetic_result {
    using type = decltype(std::declval<T>() + std::declval<U>());
};

template <typename T, typename U>
using arithmetic_result_t = typename arithmetic_result<T, U>::type;

template <typename A, typename B = int, typename C = int, typename D = int>
using enable_if_arithmetic_t = std::enable_if_t<
    std::is_arithmetic<A>::value &&
    std::is_arithmetic<B>::value &&
    std::is_arithmetic<C>::value &&
    std::is_arithmetic<D>::value>;

template <template <typename T> class VECTOR, typename T> 
struct TVecAddOperators 
{
public:

    template <typename OHTER>
    VECTOR<T>& operator+=(const VECTOR<OHTER>& v)
    {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++)
            lhs[i] += v[i];
        return lhs;
    }

    template <typename OHTER>
    VECTOR<T>& operator-=(const VECTOR<OHTER>& v)
    {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++)
            lhs[i] -= v[i];
        return lhs;
    }

private:

    template <typename U>
    friend inline 
    VECTOR<T> operator+(const VECTOR<T>& lv, const VECTOR<U>& rv)
    {
        VECTOR<T> res(lv);
        res += rv;
        return res;
    }

    template <typename U, typename = enable_if_arithmetic_t<U>>
    friend inline 
    VECTOR<T> operator+(const VECTOR<T>& lv, U rv)
    {
        return lv + VECTOR<U>(rv);
    }

    template <typename U, typename = enable_if_arithmetic_t<U>>
    friend inline 
    VECTOR<T> operator+(U lv, const VECTOR<T>& rv)
    {
        return VECTOR<U>(lv) + rv;
    }

    template <typename U>
    friend inline 
    VECTOR<T> operator-(const VECTOR<T>& lv, const VECTOR<U>& rv)
    {
        VECTOR<T> res(lv);
        res -= rv;
        return res;
    }

    template <typename U, typename = enable_if_arithmetic_t<U>>
    friend inline 
    VECTOR<T> operator-(const VECTOR<T>& lv, U rv)
    {
        return lv - VECTOR<U>(rv);
    }

    template <typename U, typename = enable_if_arithmetic_t<U>>
    friend inline 
    VECTOR<T> operator-(U lv, const VECTOR<T>& rv)
    {
        return VECTOR<U>(lv) - rv;
    }
};

template <template <typename T> class VECTOR, typename T> 
struct TVecProductOperators 
{
public:

    template <typename OHTER>
    VECTOR<T>& operator*=(const VECTOR<OHTER>& v)
    {
        VECTOR<T>& lhs = static_cast<VECTOR<T>&>(*this);
        for (size_t i = 0; i < lhs.size(); i++)
            lhs[i] *= v[i];
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

private:

    template <typename U, typename = enable_if_arithmetic_t<U>>
    friend inline 
    VECTOR<arithmetic_result_t<T, U>> operator*(const VECTOR<T>& lv, const VECTOR<U>& rv)
    {
        VECTOR<arithmetic_result_t<T, U>> res(lv);
        res *= rv;
        return res;
    }

    template <typename U, typename = enable_if_arithmetic_t<U>>
    friend inline 
    VECTOR<T> operator*(U lv, const VECTOR<T>& rv)
    {
        return VECTOR<U>(lv) * rv;
    }

    template <typename U, typename = enable_if_arithmetic_t<U>>
    friend inline 
    VECTOR<T> operator*(const VECTOR<T>& lv, U rv)
    {
        return lv * VECTOR<U>(rv);
    }

    template <typename U, typename = enable_if_arithmetic_t<U>>
    friend inline 
    VECTOR<T> operator/(const VECTOR<T>& lv, const VECTOR<U>& rv)
    {
        VECTOR<T> res(lv);
        res /= rv;
        return res;
    }

    template <typename U, typename = enable_if_arithmetic_t<U>>
    friend inline 
    VECTOR<T> operator/(const VECTOR<T>& lv, U rv)
    {
        return lv / VECTOR<U>(rv);
    }

    template <typename U, typename = enable_if_arithmetic_t<U>>
    friend inline 
    VECTOR<T> operator/(U lv, const VECTOR<T>& rv)
    {
        return VECTOR<U>(lv) / rv;
    }
};

template <template <typename T> class VECTOR, typename T>
struct TVecCompareOperators
{
private:

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

template <template <typename T> class VECTOR, typename T>
struct TVecFunctions
{
    friend inline VECTOR<T> clamp(const VECTOR<T>& v, T min, T max) 
    {
        VECTOR<T> res(v);
        for (size_t i = 0; i < v.size(); i++) {
            res[i] = el::min(max, el::max(min, res[i]));
        }
        return res;
    }

    friend inline VECTOR<T> mix(const VECTOR<T>& u, const VECTOR<T>& v, T t) 
    {
        VECTOR<T> res{};
        for (size_t i = 0; i < res.size(); i++) {
            res[i] = u[i] + (v[i] - u[i]) * t;
        }
        return res;
    }
};

template <typename T>
class EL_EMPTY_BASES TVec2 :
        public TVecAddOperators<TVec2, T>,
        public TVecProductOperators<TVec2, T>,
        public TVecCompareOperators<TVec2, T>,
        public TVecFunctions<TVec2, T>
{
public:

    using value_type = T;

    static constexpr size_t SIZE = 2;

    TVec2() noexcept = default;

    template <typename A, typename = enable_if_arithmetic_t<A>>
    TVec2(A a) noexcept : v{ T(a), T(a) } {}

    inline size_t size() const { return SIZE; }

    inline const T& operator[](size_t i) const noexcept {
        assert(i < SIZE);
        return v[i];
    }

    inline T& operator[](size_t i) noexcept {
        assert(i < SIZE);
        return v[i];
    }

    union {
        T v[SIZE];
        struct { T y, z; };
        struct { T g, b; };
    };
};

template <typename T>
class EL_EMPTY_BASES TVec3 :
        public TVecAddOperators<TVec3, T>,
        public TVecProductOperators<TVec3, T>,
        public TVecCompareOperators<TVec3, T>,
        public TVecFunctions<TVec3, T>
{
public:

    using value_type = T;

    static constexpr size_t SIZE = 3;

    TVec3() noexcept = default;

    template <typename A, typename = enable_if_arithmetic_t<A>>
    TVec3(A a) noexcept : v{ T(a), T(a), T(a) } {}

    template <typename A, typename B, typename C, typename = enable_if_arithmetic_t<A, B, C>>
    TVec3(A a, B b, C c) noexcept : v{ T(a), T(b), T(c) } {}

    inline size_t size() const { return SIZE; }

    inline const T& operator[](size_t i) const noexcept {
        assert(i < SIZE);
        return v[i];
    }

    inline T& operator[](size_t i) noexcept {
        assert(i < SIZE);
        return v[i];
    }

    union {
        T v[SIZE];
#if EL_SUPPORT_UNRESTRICTED_UNIONS
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
#endif // EL_SUPPORT_UNRESTRICTED_UNIONS
    };
};

template <typename T>
class EL_EMPTY_BASES TVec4 :
    public TVecAddOperators<TVec4, T>,
    public TVecProductOperators<TVec4, T>,
    public TVecCompareOperators<TVec4, T>,
    public TVecFunctions<TVec4, T>
{
public:

    using value_type = T;

    static constexpr size_t SIZE = 4;

    TVec4() noexcept = default;

    template <typename A, typename = enable_if_arithmetic_t<A>>
    TVec4(A a) noexcept : v{ T(a), T(a), T(a), T(a) } {}

    template <typename A, typename B, typename C, typename D,
        typename = enable_if_arithmetic_t<A,B,C,D>>
    TVec4(A a, B b, C c, D d) noexcept : v{ T(a), T(b), T(c), T(d) } {}

    template <typename A, typename B, typename = enable_if_arithmetic_t<A, B>>
    TVec4(const TVec3<A>& a, B b) noexcept : v{ T(a[0]), T(a[1]), T(a[2]), T(b)} {}

    inline size_t size() const { return SIZE; }

    inline const T& operator[](size_t i) const noexcept {
        assert(i < SIZE);
        return v[i];
    }

    inline T& operator[](size_t i) noexcept {
        assert(i < SIZE);
        return v[i];
    }

    union {
        T v[SIZE];
#if EL_SUPPORT_UNRESTRICTED_UNIONS
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
#endif // EL_SUPPORT_UNRESTRICTED_UNIONS
    };
};

template<typename MATRIX_R, typename MATRIX_A, typename MATRIX_B,
            typename = std::enable_if_t<
                MATRIX_A::NUM_COLS == MATRIX_B::NUM_ROWS &&
                MATRIX_R::NUM_COLS == MATRIX_B::NUM_COLS &&
                MATRIX_R::NUM_ROWS == MATRIX_B::NUM_ROWS>>
MATRIX_R multiply(const MATRIX_A& lhs, const MATRIX_B& rhs)
{
    MATRIX_R res{};
    for (size_t col = 0; col < MATRIX_R::NUM_COLS; ++col) {
        res[col] = lhs * rhs[col];
    }
    return res;
}

template <typename T>
struct TMat44
{
    enum NoInit { NO_INIT };

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

    col_type m_cols[NUM_COLS];

    explicit TMat44(NoInit) noexcept {}

    TMat44() noexcept;

    template <typename A, typename = enable_if_arithmetic_t<A>>
    explicit TMat44(A v) noexcept;

    template <typename A>
    explicit TMat44(const TVec4<A>& v) noexcept;

    template <
        typename A, typename B, typename C, typename D,
        typename E, typename F, typename G, typename H,
        typename I, typename J, typename K, typename L,
        typename M, typename N, typename O, typename P>
    explicit TMat44(A m00, B m01, C m02, D m03,
                    E m10, F m11, G m12, H m13,
                    I m20, J m21, K m22, L m23,
                    M m30, N m31, O m32, P m33) noexcept;

    const col_type& operator[](size_t col) const
    {
        assert(col < NUM_COLS);
        return m_cols[col];
    }

    col_type& operator[](size_t col)
    {
        assert(col < NUM_COLS);
        return m_cols[col];
    }

    template<typename A>
    static TMat44 translation(const TVec3<A>& t) noexcept {
        TMat44 r;
        r[3] = TVec4<T>{ t, 1 };
        return r;
    }

    template<typename A>
    static TMat44 scaling(const TVec3<A>& s) noexcept {
        return TMat44{ TVec4<T>{ s, 1 }};
    }

private:

    template <typename U>
    friend inline TMat44<arithmetic_result_t<T, U>>
    operator*(const TMat44<T>& lv, const TMat44<U>& rv)
    {
        return multiply<TMat44<arithmetic_result_t<T, U>>>(lv, rv);
    }

    // matrix * vector
    template <typename U>
    friend inline typename TMat44<arithmetic_result_t<T, U>>::col_type
    operator*(const TMat44<T>& lv, const TVec4<U>& rv)
    {
        typename TMat44<arithmetic_result_t<T, U>>::col_type res{};
        for (size_t col = 0; col < TMat44::NUM_COLS; ++col) {
            res += lv[col] * rv[col];
        }
        return res;
    }
};

template <typename T>
TMat44<T>::TMat44() noexcept
    : m_cols {
        col_type(1, 0, 0, 0),
        col_type(0, 1, 0, 0),
        col_type(0, 0, 1, 0),
        col_type(0, 0, 0, 1) } {
}

template <typename T>
template <typename A,typename>
TMat44<T>::TMat44(A v) noexcept 
    : m_cols {
        col_type(v, 0, 0, 0),
        col_type(0, v, 0, 0),
        col_type(0, 0, v, 0),
        col_type(0, 0, 0, v) } {
}

template <typename T>
template <typename A>
TMat44<T>::TMat44(const TVec4<A>& v) noexcept
    : m_cols {
        col_type(v[0], 0, 0, 0),
        col_type(0, v[1], 0, 0),
        col_type(0, 0, v[2], 0),
        col_type(0, 0, 0, v[3]) } {
}

template <typename T>
template <
    typename A, typename B, typename C, typename D,
    typename E, typename F, typename G, typename H,
    typename I, typename J, typename K, typename L,
    typename M, typename N, typename O, typename P>
TMat44<T>::TMat44(A m00, B m01, C m02, D m03,
                    E m10, F m11, G m12, H m13,
                    I m20, J m21, K m22, L m23,
                    M m30, N m31, O m32, P m33) noexcept
{
    m_cols[0] = col_type(m00, m01, m02, m03);
    m_cols[1] = col_type(m10, m11, m12, m13);
    m_cols[2] = col_type(m20, m21, m22, m23);
    m_cols[3] = col_type(m30, m31, m32, m33);
}

} // namespace detail

template <typename T, typename = details::enable_if_arithmetic_t<T>> using vec2 = details::TVec2<T>;
template <typename T, typename = details::enable_if_arithmetic_t<T>> using vec3 = details::TVec3<T>;
template <typename T, typename = details::enable_if_arithmetic_t<T>> using vec4 = details::TVec4<T>;

using double2 = vec2<double>;
using float2 = vec2<float>;

using double3 = vec3<double>;
using float3 = vec3<float>;

using double4 = vec4<double>;
using float4 = vec4<float>;

using mat4 = details::TMat44<double>;
using mat4f = details::TMat44<float>;

} // namespace el

#endif // __EL_MATH_VEC4_H__