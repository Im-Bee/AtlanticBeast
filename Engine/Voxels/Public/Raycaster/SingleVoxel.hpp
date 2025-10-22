#ifndef AB_VOXEL_H
#define AB_VOXEL_H

#include "Voxels.hpp"

namespace Voxels
{

struct alignas(16) Voxel
{
    constexpr const static uint32_t MaxPerInstance = 3;

    uint32_t Type;
    uint32_t Id1;
    uint32_t Id2;
    uint32_t Id3;
};

} // !Voxels

#endif // !AB_VOXEL_H

