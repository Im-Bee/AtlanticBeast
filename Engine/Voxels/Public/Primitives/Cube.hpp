#ifndef AB_CUBE_H
#define AB_CUBE_H

#include "Math/Math.hpp"
#include "Primitives/Object.hpp"
#include <cstdint>

namespace Voxels
{

class alignas(16) Cube : public WorldObject
{
public:

    Cube() 
        : m_fHalfSizeX(.24f)
        , m_fHalfSizeY(.24f)
        , m_fHalfSizeZ(.24f)
        , m_uColor(0x4422DDFF)
        , WorldObject()
    { 
        this->SetRotation(Rot3(.9));
    }

    Cube& operator=(const Cube&) noexcept = default;
    Cube& operator=(Cube&&) noexcept = default;

    Cube(const Cube&) = default;
    Cube(Cube&&) = default;

    ~Cube() = default;

public:

    Vec3 GetHalfSize() const
    { return Vec3(m_fHalfSizeX, m_fHalfSizeY, m_fHalfSizeZ); }

public:

    void SetColor(uint32_t color) 
    { m_uColor = color | 0x000000FF; }

private:

    uint32_t m_uColor;  
    float    m_fHalfSizeX;  
    float    m_fHalfSizeY;  
    float    m_fHalfSizeZ;  

};

} // !Voxels
#endif // !AB_CUBE_H
