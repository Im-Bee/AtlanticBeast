#ifndef AB_PUSH_CONSTANTS_H
#define AB_PUSH_CONSTANTS_H

#include "Voxels.hpp"

namespace Voxels
{

struct alignas(16) VoxelPushConstants
{
    float viewProjInverse[16];
    float cameraPos[3];
    int gridSize[3];
    int padding;
};

} // !Voxels


#endif // !AB_PUSH_CONSTANTS_H
