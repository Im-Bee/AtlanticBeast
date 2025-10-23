#ifndef AB_CUBE_H
#define AB_CUBE_H

#include "Math/Math.hpp"
#include "Primitives/Object.hpp"

namespace Voxels
{

class alignas(16) Cube : public WorldObject
{
public:

    Cube() 
        : m_fHalfSize(0.5f)
        , m_uColor(0xFF00FFFF)
        , WorldObject()
    { }

    ~Cube() = default;

public:

    Vec3 GetHalfSize() const
    { return Vec3(m_fHalfSize, m_fHalfSize, m_fHalfSize); }

private:

    uint32_t m_uColor;  
    float    m_fHalfSize;  
    uint32_t m_uPadding2;  
    uint32_t m_uPadding3;  

};

} // !Voxels
#endif // !AB_CUBE_H
