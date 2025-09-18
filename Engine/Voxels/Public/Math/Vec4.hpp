#ifndef AB_VEC4_H
#define AB_VEC4_H

namespace Voxels
{

struct alignas(16) Vec4
{ 
    explicit Vec4(float x = 0, float y = 0, float z = 0, float w = 0)
        : x(x), y(y), z(z), w(w)
    { }

    float x;
    float y;
    float z;
    float w;
};

struct alignas(16) iVec4
{ 
    explicit iVec4(int32_t x = 0, int32_t y = 0, int32_t z = 0, int32_t w = 0)
        : x(x), y(y), z(z), w(w)
    { }

    int32_t x;
    int32_t y;
    int32_t z;
    int32_t w;
};

} // !Voxels
  
#endif // !AB_VEC4_H

