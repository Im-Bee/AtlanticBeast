#ifndef AB_VOXEL_H
#define AB_VOXEL_H

#include "Voxels.hpp"

namespace Voxels
{

struct alignas(16) Voxel
{
    uint32_t Type;
    uint32_t RGBA;
};

} // !Voxels

#endif // !AB_VOXEL_H

