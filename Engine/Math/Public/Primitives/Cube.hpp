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
        , WorldObject()
    { }

public:

    Vec3 GetHalfSize() const
    { return m_fHalfSize; }

public:

    void SetHalfSize(Vec3 halfSize)
    { m_fHalfSize = halfSize; }

private:

    Vec3 m_fHalfSize;

};

} // !Voxels
#endif // !AB_CUBE_H
