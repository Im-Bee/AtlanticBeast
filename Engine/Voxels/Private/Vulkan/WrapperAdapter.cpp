#include "Vulkan/WrapperAdapter.hpp"

namespace Voxels
{

WrapperAdapter::~WrapperAdapter()
{
	if (m_Device != VK_NULL_HANDLE) {
		vkDestroyDevice(m_Device, NULL);
		m_Device = VK_NULL_HANDLE;
	}
}

void WrapperAdapter::RecreateAdapter(uint32_t uQueueIndex,
									 VkDevice device,
									 VkQueue queue)
{
	if (m_Device != VK_NULL_HANDLE) {
		vkDestroyDevice(m_Device, NULL);
	}

	m_uQueueFamily = uQueueIndex;
	m_Device = device;
	m_Queue = queue;
}

} //!Voxels