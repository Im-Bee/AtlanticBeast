#ifndef AB_FRAME_RESOURCES_H
#define AB_FRAME_RESOURCES_H

#include "Voxels.hpp"

namespace Voxels
{

constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;

struct FrameResources
{
	VkFence			InFlightFence;
	VkSemaphore		ImageAvailable;
	VkSemaphore		RenderFinished;
	VkCommandBuffer CommandBuffer;
};

} // !Voxels

#endif // !AB_FRAME_RESOURCES_H
