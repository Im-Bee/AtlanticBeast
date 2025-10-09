#include "Vulkan/ComputeAdapter.hpp"

namespace Voxels
{

using namespace std;

// --------------------------------------------------------------------------------------------------------------------
ComputeAdapter::ComputeAdapter(shared_ptr<const Hardware> gpu)
    : Adapter(gpu->GetPhysicalDevice(), 
                     VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT,
                     GetExtensions(),
                     GetFeaturesImpl())
    , m_pGPU(gpu)
{ }

// --------------------------------------------------------------------------------------------------------------------
const ::std::vector<const char*>& ComputeAdapter::GetExtensionsImpl() const
{
    static const std::vector<const char*> vpszDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    return vpszDeviceExtensions;
}

// --------------------------------------------------------------------------------------------------------------------
void* ComputeAdapter::GetFeaturesImpl() const
{
    static bool bCreated = false;

    static VkPhysicalDeviceTimelineSemaphoreFeatures   semaphoreFeatures;
    static VkPhysicalDevice8BitStorageFeatures         bitStorageFeatures;
    static VkPhysicalDeviceVulkanMemoryModelFeatures   memoryModelFeatures;

    if (bCreated) {
        return &memoryModelFeatures;
    }

    semaphoreFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES;
    semaphoreFeatures.pNext = NULL;
    semaphoreFeatures.timelineSemaphore = VK_TRUE;

    bitStorageFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR;
    bitStorageFeatures.pNext = &semaphoreFeatures;
    bitStorageFeatures.storageBuffer8BitAccess  = VK_TRUE;
    bitStorageFeatures.storagePushConstant8     = VK_FALSE;
    bitStorageFeatures.uniformAndStorageBuffer8BitAccess = VK_FALSE;

    memoryModelFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES;
    memoryModelFeatures.pNext = &bitStorageFeatures;
    memoryModelFeatures.vulkanMemoryModel = VK_TRUE;
    memoryModelFeatures.vulkanMemoryModelDeviceScope = VK_TRUE;
    memoryModelFeatures.vulkanMemoryModelAvailabilityVisibilityChains = VK_FALSE;

    bCreated = true;

    return &memoryModelFeatures;
}

};
