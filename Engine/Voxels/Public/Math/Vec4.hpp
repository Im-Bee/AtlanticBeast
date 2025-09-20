#ifndef AB_VEC4_H
#define AB_VEC4_H

namespace Voxels
{

// ---------------------------------------------------------------------------------------------------------------------
struct alignas(16) Vec4
{ 
    explicit Vec4(float x = 0, float y = 0, float z = 0, float w = 0)
        : x(x), y(y), z(z), w(w)
    { }

    explicit Vec4(const Vec3& vec3, float w = 0)
        : x(vec3.x), y(vec3.y), z(vec3.z), w(w)
    { }

    float x;
    float y;
    float z;
    float w;

    static constexpr size_t Size = 4;

    constexpr float operator[](size_t uIndex) const
    {
        AB_ASSERT(uIndex < Size);

        if (uIndex == 0) {
            return x;
        }
        if (uIndex == 1) {
            return y;
        }
        if (uIndex == 2) {
            return z;
        }
        return w;
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
        if (uIndex == 2) {
            return z;
        }
        return w;
    }
};

// ---------------------------------------------------------------------------------------------------------------------
struct alignas(16) iVec4
{ 
    explicit iVec4(int32_t x = 0, int32_t y = 0, int32_t z = 0, int32_t w = 0)
        : x(x), y(y), z(z), w(w)
    { }

    int32_t x;
    int32_t y;
    int32_t z;
    int32_t w;

    static constexpr size_t Size = 4;
    
    constexpr int32_t operator[](size_t uIndex) const
    {
        AB_ASSERT(uIndex < Size);

        if (uIndex == 0) {
            return x;
        }
        if (uIndex == 1) {
            return y;
        }
        if (uIndex == 2) {
            return z;
        }
        return w;
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
        if (uIndex == 2) {
            return z;
        }
        return w;
    }
};

} // !Voxels
  
#endif // !AB_VEC4_H

