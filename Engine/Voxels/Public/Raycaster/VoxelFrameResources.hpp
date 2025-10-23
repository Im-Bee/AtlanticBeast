#ifndef AB_VOXEL_FRAME_RESOURCES_H
#define AB_VOXEL_FRAME_RESOURCES_H

#include "Vulkan/FrameResources.hpp"
#include "Vulkan/GPUBuffer.hpp"
#include "Vulkan/GPUStreamBuffer.hpp"

namespace Voxels
{
    
struct VoxelFrameResources : public FrameResources
{
    GPUBuffer VoxelBuffer;
    GPUBuffer CubeBuffer;    
    GPUStreamBuffer StageVoxelBuffer;
    GPUStreamBuffer StageCubeBuffer;
};

} // !Voxels
#endif //!AB_VOXEL_FRAME_RESOURCES_H
