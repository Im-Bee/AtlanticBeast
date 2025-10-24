#ifndef AB_CUBE_H
#define AB_CUBE_H

#include "Math/Math.hpp"
#include "Math/Vec3.hpp"
#include "Primitives/Object.hpp"

namespace Voxels
{

class alignas(16) Cube : public WorldObject
{
public:

    Cube() 
        : m_fHalfSize(.33f, .33f, .33f)
        , m_uColor(0x4422DDFF)
        , WorldObject()
    { 
        this->SetRotation(Rot3(2.9, 1., 0.1));
    }

    Cube& operator=(const Cube&) noexcept = default;
    Cube& operator=(Cube&&) noexcept = default;

    Cube(const Cube&) = default;
    Cube(Cube&&) = default;

    ~Cube() = default;

public:

    Vec3 GetHalfSize() const
    { return m_fHalfSize; }

public:

    void SetColor(uint32_t color) 
    { m_uColor = color | 0x000000FF; }

private:

    uint32_t m_uColor;  
    Vec3     m_fHalfSize;
};

} // !Voxels
#endif // !AB_CUBE_H
