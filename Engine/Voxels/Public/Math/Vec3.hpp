#ifndef AB_VEC3_H
#define AB_VEC3_H

#include "CSystem.hpp"

namespace Voxels
{

// ---------------------------------------------------------------------------------------------------------------------
struct alignas(16) Vec3
{
    Vec3()
        : x(0.f), y(0.f), z(0.f)
    { } 

    explicit Vec3(float x, float y = 0, float z = 0)
        : x(x), y(y), z(z)
    { }

    explicit Vec3(uint32_t x, uint32_t y = 0, uint32_t z = 0)
        : x(static_cast<float>(x)), y(static_cast<float>(y)), z(static_cast<float>(z))
    { }

    float x;
    float y;
    float z;
    float _Padding;

    static constexpr size_t Size = 3;
    
    constexpr float& operator[](size_t uIndex);

    constexpr float operator[](size_t uIndex) const;

    inline Vec3& operator+=(const Vec3& vB);

    inline Vec3 operator+(const Vec3& vB) const;

    inline Vec3 operator*(const Vec3& vB) const;

    inline Vec3 operator*(const float vB) const;
};

// ---------------------------------------------------------------------------------------------------------------------
struct alignas(16) iVec3
{
    iVec3()
        : x(0), y(0), z(0)
    { }

    explicit iVec3(int32_t x, int32_t y = 0, int32_t z = 0)
        : x(x), y(y), z(z)
    { }

    explicit iVec3(float x, float y = 0.f, float z = 0.f)
        : x(static_cast<uint32_t>(x)), y(static_cast<uint32_t>(y)), z(static_cast<uint32_t>(z))
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
