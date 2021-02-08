#include "Mat4.h"
#include <cmath>

namespace gszauer {

bool MATH_PURE operator==(const mat4& a, const mat4& b) 
{
    #pragma nounroll
    for (size_t i = 0; i < 16; i++) {
        if (fabsf(a.v[i] - b.v[i]) > MAT4_EPSILON) {
            return false;
        }
    }
    return true;
}

bool MATH_PURE operator!=(const mat4& a, const mat4& b)
{
    return !(a == b);
}


} // namespace gszauer
