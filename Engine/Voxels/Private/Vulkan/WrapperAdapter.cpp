#include "Vulkan/WrapperAdapter.hpp"

#include "Vulkan/ErrorHandling.hpp"

namespace Voxels 
{

using namespace std;

uint32_t WrapperAdapter::ChooseQueueFamily(VkPhysicalDevice gpu, uint32_t uFlags)
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

VkDevice WrapperAdapter::CreateDevice(VkPhysicalDevice gpu,
                                      const std::vector<const char*>& vExtensions,
                                      const void* pFeatures,
                                      uint32_t uFamilyIndex)
{
    VkDevice                                            device                              = VK_NULL_HANDLE;
    VkDeviceCreateInfo                                  createInfo;
    VkDeviceQueueCreateInfo                             queueCreateInfo;
    VkPhysicalDeviceFeatures                            deviceFeatures;
    float                                               queuePriorities[]                  = { 1. };

    queueCreateInfo.sType               = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext               = NULL;
    queueCreateInfo.flags               = 0;
    queueCreateInfo.queueFamilyIndex    = uFamilyIndex;
    queueCreateInfo.pQueuePriorities    = queuePriorities;
    queueCreateInfo.queueCount          = sizeof(queuePriorities) / sizeof(float);

    memset(&deviceFeatures, 0, sizeof(VkPhysicalDeviceFeatures));
    
    deviceFeatures.fragmentStoresAndAtomics         = VK_TRUE;
    deviceFeatures.vertexPipelineStoresAndAtomics   = VK_TRUE;
    deviceFeatures.shaderInt64                      = VK_TRUE;

    createInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext                    = pFeatures;
    createInfo.flags                    = 0;
    createInfo.pQueueCreateInfos        = &queueCreateInfo;
    createInfo.queueCreateInfoCount     = 1;
    createInfo.ppEnabledLayerNames      = NULL;
    createInfo.enabledLayerCount        = 0;
    createInfo.ppEnabledExtensionNames  = !vExtensions.empty() ? &vExtensions[0] : NULL;
    createInfo.enabledExtensionCount    = static_cast<uint32_t>(!vExtensions.empty() ? vExtensions.size() : 0);
    createInfo.pEnabledFeatures         = &deviceFeatures;

    THROW_IF_FAILED(vkCreateDevice(gpu,
                                   &createInfo,
                                   NULL,
                                   &device));

    return device;
}

VkQueue WrapperAdapter::CreateQueue(VkDevice dv, uint32_t uQueueIndex)
{
    VkQueue graphicsQueue;

    vkGetDeviceQueue(dv, uQueueIndex, 0, &graphicsQueue);

    return graphicsQueue;
}

} // !Voxels
