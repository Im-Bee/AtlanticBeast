#ifndef AB_VEC3_OPERATORS_H
#define AB_VEC3_OPERATORS_H

#include "Vec3.hpp"
#include "Operations.hpp"

namespace Voxels
{
    
// --------------------------------------------------------------------------------------------------------------------
constexpr float& Vec3::operator[](size_t uIndex)
{
    AB_ASSERT(uIndex < Size);

    if (uIndex == 0) {
        return x;
    }
    if (uIndex == 1) {
        return y;
    }
    return z;
} 

// --------------------------------------------------------------------------------------------------------------------
constexpr float Vec3::operator[](size_t uIndex) const
{
    AB_ASSERT(uIndex < Size);

    if (uIndex == 0) {
        return x;
    }
    if (uIndex == 1) {
        return y;
    }
    return z;
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3& Vec3::operator+=(const Vec3& vB)
{
    return AddAssign(*this, vB);
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 Vec3::operator*(const Vec3& vB) const
{
    return Multiply(*this, vB);
}

// --------------------------------------------------------------------------------------------------------------------
inline Vec3 Vec3::operator*(const float vB) const
{
    return MultiplyScalar(*this, vB);
}

} //!Volxes

#endif //!AB_VEC3_OPERATORS_H
