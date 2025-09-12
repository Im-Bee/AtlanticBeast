#include "DeviceAdapter.hpp"

#include "ErrorHandling.hpp"

namespace Voxels
{

using namespace std;

// DeviceAdapter // ----------------------------------------------------------------------------------------------------
DeviceAdapter::DeviceAdapter(shared_ptr<const Hardware> gpu)
    : m_pHardware(gpu)
    , m_Device(CreateDeviceAdapter(m_pHardware))
{ }

// ---------------------------------------------------------------------------------------------------------------------
DeviceAdapter::~DeviceAdapter()
{ 
    if (m_Device != VK_NULL_HANDLE) {
        vkDestroyDevice(m_Device, nullptr);
        m_Device = VK_NULL_HANDLE;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
VkDevice DeviceAdapter::CreateDeviceAdapter(shared_ptr<const Hardware>& gpu)
{ 
    VkDevice                                            device                                  = VK_NULL_HANDLE;
    VkDeviceCreateInfo                                  createInfo;
    VkDeviceQueueCreateInfo                             queueCreateInfo;
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR       rayTracingPipelineFeatures;
    VkPhysicalDeviceAccelerationStructureFeaturesKHR    accelerationStructureFeatures;
    VkPhysicalDeviceBufferDeviceAddressFeatures         bufferDeviceAddressFeatures;
    float                                               queuePriorities[]                       = { 1. };

    const std::vector<const char*> vpszDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
        VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
        VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
        VK_KHR_SPIRV_1_4_EXTENSION_NAME,
        VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME,
    };
    
    rayTracingPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
    rayTracingPipelineFeatures.pNext                                                    = NULL;
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
    queueCreateInfo.queueFamilyIndex    = FindQueueFamilyIndex(gpu);
    queueCreateInfo.pQueuePriorities    = queuePriorities;
    queueCreateInfo.queueCount          = sizeof(queuePriorities) / sizeof(float);

    createInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext                    = &bufferDeviceAddressFeatures;
    createInfo.flags                    = 0;
    createInfo.pQueueCreateInfos        = &queueCreateInfo;
    createInfo.queueCreateInfoCount     = 1;
    createInfo.ppEnabledLayerNames      = NULL;
    createInfo.enabledLayerCount        = 0;
    createInfo.ppEnabledExtensionNames  = &vpszDeviceExtensions[0];
    createInfo.enabledExtensionCount    = vpszDeviceExtensions.size();
    createInfo.pEnabledFeatures         = NULL;

    ThrowIfFailed(vkCreateDevice(gpu->GetPhysicalDevice(),
                                 &createInfo,
                                 NULL,
                                 &device));

    return device;
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


} // !Voxels

