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
    Vector result = Vector();
    float mod = 0.f;
    float invMag;

    for (size_t i = 0; i < Vector::Size; ++i) 
        mod += v[i] * v[i];

    if (mod == 0.f) {
        return result;
    }

    invMag = 1.f / ::std::sqrtf(mod);

    for (size_t i = 0; i < Vector::Size; ++i)
        result[i] = v[i] * invMag;

    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
constexpr inline Vector RotateX(const Vector& v, float angleRad)
{
    if constexpr (Vector::Size == 3) {
        Vector result;
        float c = std::cos(angleRad);
        float s = std::sin(angleRad);

        result.x = v.x;
        result.y = v.y * c - v.z * s;
        result.z = v.y * s + v.z * c;

        return result;
    }

    static_assert(true, "This size of a vector doesn't have impementation of rotate X yet");
}

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
constexpr inline Vector RotateY(const Vector& v, float angleRad)
{
    if constexpr (Vector::Size == 3) {
        Vector result;
        float c = std::cos(angleRad);
        float s = std::sin(angleRad);

        result.x = v.x * c + v.z * s;
        result.y = v.y;
        result.z = -v.x * s + v.z * c;

        return result;
    }

    static_assert(true, "This size of a vector doesn't have impementation of rotate Y yet");
}

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
constexpr inline Vector& AddAssign(Vector& vA, const Vector& vB)
{
    if constexpr (Vector::Size == 3) {
        vA.x += vB.x;
        vA.y += vB.y;
        vA.z += vB.z;
        return vA;
    }

    static_assert(true, "This size of a vector doesn't have impementation of addition product yet");
}

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
constexpr inline Vector Multiply(const Vector& vA, const Vector& vB)
{
    if constexpr (Vector::Size == 3) {
        Vector r;

        r.x = vA.x * vB.x;
        r.y = vA.y * vB.y;
        r.z = vA.z * vB.z;

        return r;
    }

    static_assert(true, "This size of a vector doesn't have impementation of addition product yet");
}

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
constexpr inline Vector MultiplySingle(const Vector& vA, const float vB)
{
    if constexpr (Vector::Size == 3) {
        Vector r;

        r.x = vA.x * vB;
        r.y = vA.y * vB;
        r.z = vA.z * vB;

        return r;
    }

    static_assert(true, "This size of a vector doesn't have impementation of addition product yet");
}

} // !Voxels
  
#endif // !AB_OPERATIONS_H

