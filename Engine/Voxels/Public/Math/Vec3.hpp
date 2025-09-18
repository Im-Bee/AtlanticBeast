#ifndef AB_VEC3_H
#define AB_VEC3_H

#include "CSystem.hpp"

namespace Voxels
{

// ---------------------------------------------------------------------------------------------------------------------
struct alignas(16) Vec3
{
    explicit Vec3(float x = 0, float y = 0, float z = 0)
        : x(x), y(y), z(z)
    { }

    float x;
    float y;
    float z;
    float _Padding;

    static constexpr size_t Size = 3;
    
    constexpr float operator[](size_t uIndex) const
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

    constexpr float& operator[](size_t uIndex)
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
};

// ---------------------------------------------------------------------------------------------------------------------
struct alignas(16) iVec3
{
    explicit iVec3(int32_t x = 0, int32_t y = 0, int32_t z = 0)
        : x(x), y(y), z(z)
    { }

    int32_t x;
    int32_t y;
    int32_t z;
    int32_t _Padding;

    static constexpr size_t Size = 3;
    
    constexpr int32_t operator[](size_t uIndex) const
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

    constexpr int32_t& operator[](size_t uIndex)
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
};

} // !Voxels

#endif // !AB_VEC3_H
