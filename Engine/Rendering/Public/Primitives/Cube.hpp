#ifndef AB_CUBE_H
#define AB_CUBE_H

#include "Math.hpp"
#include "Primitives/Object.hpp"

namespace Voxels
{

class alignas(16) Cube : public WorldObject
{
public:

    Cube() 
        : m_fHalfSize(.5f, .5f, .5f)
        , m_uColor(0xFFFFFFFF)
        , WorldObject()
    { }

public:

    Cube& operator=(const Cube&) noexcept = default;
    Cube& operator=(Cube&&) noexcept = default;

    Cube(const Cube&) = default;
    Cube(Cube&&) = default;

public:

    ~Cube() = default;

public:

    Vec3 GetHalfSize() const
    { return m_fHalfSize; }

public:

    void SetColor(uint32_t color) 
    { m_uColor = color | 0x000000FF; }

private:

    Vec3     m_fHalfSize;
    uint32_t m_uColor;  
};

} // !Voxels
#endif // !AB_CUBE_H
