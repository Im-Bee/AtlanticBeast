#ifndef AB_PUSH_CONSTANTS_H
#define AB_PUSH_CONSTANTS_H

#include "Voxels.hpp"

namespace Voxels
{

struct alignas(16) VoxelPushConstants
{
    float   CameraPos[3];
	float   Padding1;
    int32_t GridSize[3];
	int32_t Padding2;
    float   ViewProjInverse[16];
};

} // !Voxels


#endif // !AB_PUSH_CONSTANTS_H
