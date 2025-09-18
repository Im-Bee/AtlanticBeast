#ifndef AB_PUSH_CONSTANTS_H
#define AB_PUSH_CONSTANTS_H

#include "Math/Mat4.hpp"
#include "Math/Vec3.hpp"
#include "Math/Vec4.hpp"
#include "Voxels.hpp"

namespace Voxels
{

struct alignas(16) VoxelPushConstants
{
    Vec3    CameraPos;
    iVec4   GridSize;
    Vec3    CameraLookDir;
    Vec3    CameraRight;
    Vec3    CameraUp;
};

} // !Voxels


#endif // !AB_PUSH_CONSTANTS_H
