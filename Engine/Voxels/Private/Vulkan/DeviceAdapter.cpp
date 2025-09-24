#include "Vulkan/DeviceAdapter.hpp"

#include "Vulkan/ErrorHandling.hpp"

namespace Voxels
{

using namespace std;

// DeviceAdapter // ----------------------------------------------------------------------------------------------------
DeviceAdapter::DeviceAdapter(shared_ptr<const Hardware> gpu)
    : m_pHardware(gpu)
    , m_uQueueFamily(FindQueueFamilyIndex(m_pHardware))
    , m_Device(CreateDeviceAdapter(m_pHardware, m_uQueueFamily))
    , m_Queue(CreateQueue(m_Device, m_uQueueFamily))
{
    AB_LOG(Core::Debug::Info, L"Creating a device adapter!");
}

// ---------------------------------------------------------------------------------------------------------------------
DeviceAdapter::~DeviceAdapter()
{ 
    if (m_Device != VK_NULL_HANDLE) {
        vkDestroyDevice(m_Device, nullptr);
        m_Device = VK_NULL_HANDLE;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
uint32_t DeviceAdapter::FindQueueFamilyIndex(shared_ptr<const Hardware>& gpu)
{
    uint32_t                            uFamilyIndex    = 0;
    uint32_t                            uFamilyCount;
    vector<VkQueueFamilyProperties>     vProperties     = { };
    VkPhysicalDevice                    physicalDevice  = gpu->GetPhysicalDevice();

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &uFamilyCount, NULL);
    if (!uFamilyCount) {
        AB_LOG(Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!!");
        throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
    }
    vProperties.resize(uFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &uFamilyCount, &vProperties[0]);
    if (!uFamilyCount) {
        AB_LOG(Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!!");
        throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
    }

    uint32_t uFlags = VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT;
    for (uint32_t i = 0; i < uFamilyCount; ++i) {
        if (vProperties[i].queueFlags & uFlags) {
            return i;
        }
    }

    AB_LOG(Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!!");
    throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
}

// ---------------------------------------------------------------------------------------------------------------------
VkDevice DeviceAdapter::CreateDeviceAdapter(shared_ptr<const Hardware>& gpu, uint32_t uQueueIndex)
{ 
    VkDevice                                            device                                  = VK_NULL_HANDLE;
    VkDeviceCreateInfo                                  createInfo;
    VkDeviceQueueCreateInfo                             queueCreateInfo;
    VkPhysicalDeviceTimelineSemaphoreFeatures           semaphoreFeatures;
    VkPhysicalDevice8BitStorageFeatures                 bitStorageFeatures;
    VkPhysicalDeviceVulkanMemoryModelFeatures           memoryModelFeatures;
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR       rayTracingPipelineFeatures;
    VkPhysicalDeviceAccelerationStructureFeaturesKHR    accelerationStructureFeatures;
    VkPhysicalDeviceBufferDeviceAddressFeatures         bufferDeviceAddressFeatures;
    VkPhysicalDeviceFeatures                            deviceFeatures;
    float                                               queuePriorities[]                       = { 1. };

    const std::vector<const char*> vpszDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    };

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

    queueCreateInfo.sType               = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext               = NULL;
    queueCreateInfo.flags               = 0;
    queueCreateInfo.queueFamilyIndex    = uQueueIndex;
    queueCreateInfo.pQueuePriorities    = queuePriorities;
    queueCreateInfo.queueCount          = sizeof(queuePriorities) / sizeof(float);

    memset(&deviceFeatures, VK_FALSE, sizeof(VkPhysicalDeviceFeatures));
    
    deviceFeatures.fragmentStoresAndAtomics         = VK_TRUE;
    deviceFeatures.vertexPipelineStoresAndAtomics   = VK_TRUE;
    deviceFeatures.shaderInt64                      = VK_TRUE;

    createInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext                    = &bufferDeviceAddressFeatures;
    createInfo.flags                    = 0;
    createInfo.pQueueCreateInfos        = &queueCreateInfo;
    createInfo.queueCreateInfoCount     = 1;
    createInfo.ppEnabledLayerNames      = NULL;
    createInfo.enabledLayerCount        = 0;
    createInfo.ppEnabledExtensionNames  = &vpszDeviceExtensions[0];
    createInfo.enabledExtensionCount    = vpszDeviceExtensions.size();
    createInfo.pEnabledFeatures         = &deviceFeatures;

    ThrowIfFailed(vkCreateDevice(gpu->GetPhysicalDevice(),
                                 &createInfo,
                                 NULL,
                                 &device));

    return device;
}

// ---------------------------------------------------------------------------------------------------------------------
VkQueue DeviceAdapter::CreateQueue(VkDevice dv, uint32_t uQueueIndex)
{
    VkQueue graphicsQueue;

    vkGetDeviceQueue(dv, uQueueIndex, 0, &graphicsQueue);

    return graphicsQueue;
}

} // !Voxels

