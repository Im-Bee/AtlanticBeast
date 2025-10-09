#include "Vulkan/RTXDeviceAdapter.hpp"

namespace Voxels
{

using namespace std;

// RTXDeviceAdapter // -------------------------------------------------------------------------------------------------
RTXDeviceAdapter::RTXDeviceAdapter(shared_ptr<const Hardware> gpu)
    : Adapter(gpu->GetPhysicalDevice(),
                     VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT,
                     GetExtensionsImpl(),
                     GetFeaturesImpl())
    , m_pGPU(gpu)
{ }

// --------------------------------------------------------------------------------------------------------------------
const ::std::vector<const char*> RTXDeviceAdapter::GetExtensionsImpl() const
{
    static const std::vector<const char*> vpszDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    };

    return vpszDeviceExtensions;
}

// --------------------------------------------------------------------------------------------------------------------
void* RTXDeviceAdapter::GetFeaturesImpl() const
{
    static bool bCreated = false;

    static VkPhysicalDeviceTimelineSemaphoreFeatures           semaphoreFeatures;
    static VkPhysicalDevice8BitStorageFeatures                 bitStorageFeatures;
    static VkPhysicalDeviceVulkanMemoryModelFeatures           memoryModelFeatures;
    static VkPhysicalDeviceRayTracingPipelineFeaturesKHR       rayTracingPipelineFeatures;
    static VkPhysicalDeviceAccelerationStructureFeaturesKHR    accelerationStructureFeatures;
    static VkPhysicalDeviceBufferDeviceAddressFeatures         bufferDeviceAddressFeatures;

    if (bCreated) {
        return &bufferDeviceAddressFeatures;
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

    rayTracingPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
    rayTracingPipelineFeatures.pNext                                                    = &memoryModelFeatures;
    rayTracingPipelineFeatures.rayTracingPipeline                                       = VK_TRUE;
    rayTracingPipelineFeatures.rayTracingPipelineShaderGroupHandleCaptureReplay         = VK_FALSE;
    rayTracingPipelineFeatures.rayTracingPipelineShaderGroupHandleCaptureReplayMixed    = VK_FALSE;
    rayTracingPipelineFeatures.rayTracingPipelineTraceRaysIndirect                      = VK_FALSE;
    rayTracingPipelineFeatures.rayTraversalPrimitiveCulling                             = VK_TRUE;

    accelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
    accelerationStructureFeatures.pNext                                                 = &rayTracingPipelineFeatures;
    accelerationStructureFeatures.accelerationStructure                                 = VK_TRUE;
    accelerationStructureFeatures.accelerationStructureCaptureReplay                    = VK_FALSE;
    accelerationStructureFeatures.accelerationStructureIndirectBuild                    = VK_FALSE;
    accelerationStructureFeatures.accelerationStructureHostCommands                     = VK_FALSE;
    accelerationStructureFeatures.descriptorBindingAccelerationStructureUpdateAfterBind = VK_FALSE;

    bufferDeviceAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
    bufferDeviceAddressFeatures.pNext                               = &accelerationStructureFeatures;
    bufferDeviceAddressFeatures.bufferDeviceAddress                 = VK_TRUE;
    bufferDeviceAddressFeatures.bufferDeviceAddressCaptureReplay    = VK_FALSE;
    bufferDeviceAddressFeatures.bufferDeviceAddressMultiDevice      = VK_FALSE;

    bCreated = true;

    return &bufferDeviceAddressFeatures;
}

} // !Voxels

