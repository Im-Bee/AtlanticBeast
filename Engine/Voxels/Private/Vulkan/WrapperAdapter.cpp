#include "Vulkan/WrapperAdapter.hpp"

#include "Vulkan/ErrorHandling.hpp"

namespace Voxels 
{

using namespace std;

// --------------------------------------------------------------------------------------------------------------------
uint32_t Adapter::ChooseQueueFamily(VkPhysicalDevice gpu, const uint32_t uFlags) const
{
    uint32_t                            uFamilyCount;
    vector<VkQueueFamilyProperties>     vProperties     = { };
    VkPhysicalDevice                    physicalDevice  = gpu;

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

    for (uint32_t i = 0; i < uFamilyCount; ++i)
        if (vProperties[i].queueFlags & uFlags)
            return i;


    AB_LOG(Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!!");
    throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
}

// --------------------------------------------------------------------------------------------------------------------
VkDevice Adapter::CreateDevice(VkPhysicalDevice gpu,
                               const std::vector<const char*>& vExtensions,
                               const void* pFeatures,
                               const uint32_t uFamilyIndex) const
{
    VkDevice    device              = VK_NULL_HANDLE;
    float       queuePriorities[]   = { 1. };

    VkDeviceQueueCreateInfo queueCreateInfo = { };
    queueCreateInfo.sType               = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex    = uFamilyIndex;
    queueCreateInfo.pQueuePriorities    = queuePriorities;
    queueCreateInfo.queueCount          = sizeof(queuePriorities) / sizeof(float);

    VkPhysicalDeviceFeatures deviceFeatures = { };
    deviceFeatures.fragmentStoresAndAtomics         = VK_TRUE;
    deviceFeatures.vertexPipelineStoresAndAtomics   = VK_TRUE;
    deviceFeatures.shaderInt64                      = VK_TRUE;

    VkDeviceCreateInfo createInfo = { };
    createInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext                    = pFeatures;
    createInfo.pQueueCreateInfos        = &queueCreateInfo;
    createInfo.queueCreateInfoCount     = 1;
    createInfo.ppEnabledExtensionNames  = !vExtensions.empty() ? &vExtensions[0] : NULL;
    createInfo.enabledExtensionCount    = static_cast<uint32_t>(!vExtensions.empty() ? vExtensions.size() : 0);
    createInfo.pEnabledFeatures         = &deviceFeatures;

    THROW_IF_FAILED(vkCreateDevice(gpu,
                                   &createInfo,
                                   NULL,
                                   &device));

    return device;
}

// --------------------------------------------------------------------------------------------------------------------
VkQueue Adapter::CreateQueue(VkDevice dv, const uint32_t uQueueIndex) const
{
    VkQueue graphicsQueue;

    vkGetDeviceQueue(dv, uQueueIndex, 0, &graphicsQueue);

    return graphicsQueue;
}

} // !Voxels
