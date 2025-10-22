#ifndef AB_CUBE_H
#define AB_CUBE_H

#include "Math/Math.hpp"

namespace Voxels
{

struct alignas(16) CubeData 
{
    Vec3 Center;  
    Vec3 HalfSize;  
    Rot3 Rotation;  
};

class alignas(16) Cube 
{
public:

    Cube() 
        : cd({ })
    { }

    ~Cube() = default;

public:

private:

    CubeData cd;

};

} // !Voxels
#endif // !AB_CUBE_H
