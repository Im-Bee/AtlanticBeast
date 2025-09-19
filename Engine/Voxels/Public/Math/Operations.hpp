#ifndef AB_OPERATIONS_H
#define AB_OPERATIONS_H

#include "CSystem.hpp"
#include "Debug/Assert.h"

namespace Voxels
{

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
Vector Normalize(const Vector& v)
{
    Vector result = { };
    float mod = 0.f;
    float invMag;

    for (size_t i = 0; i < Vector::Size; ++i) 
        mod += result[i];

    AB_ASSERT(mod != 0.f);

    invMag = 1. / ::std::sqrt(mod);

    for (size_t i = 0; i < Vector::Size; ++i)
        result[i] = v[i] * invMag;

    return result;
}

template<class Vector>
constexpr inline Vector Cross(const Vector& vA, const Vector& vB)
{
    if constexpr (Vector::Size == 3) {
        Vector result;
        result.x = vA.y * vB.z - vB.y * vA.z;
        result.y = vA.z * vB.x - vB.z * vA.x;
        result.z = vA.x * vB.y - vB.x * vA.y;
        return result;
    }

    static_assert(true, "This size of a vector doesn't have impementation of cross product yet");
}

} // !Voxels
  
#endif // !AB_OPERATIONS_H

