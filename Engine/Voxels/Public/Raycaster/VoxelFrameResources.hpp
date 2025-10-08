#ifndef AB_VOXEL_FRAME_RESOURCES_H
#define AB_VOXEL_FRAME_RESOURCES_H

#include "Vulkan/FrameResources.hpp"
#include "Vulkan/GPUBuffer.hpp"

namespace Voxels
{
    
struct VoxelFrameResources : public FrameResources
{
    GPUBuffer VoxelBuffer;
};

} // !Voxels
#endif //!AB_VOXEL_FRAME_RESOURCES_H
