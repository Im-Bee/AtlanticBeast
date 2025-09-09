#include "Hardware.hpp"

namespace Voxels
{

using namespace std;

// Hardware // ---------------------------------------------------------------------------------------------------------
Hardware::Hardware(Instance& instance)
    : m_DeviceHandle(ChooseGPU(instance))
{ }

// ---------------------------------------------------------------------------------------------------------------------
Hardware::~Hardware()
{ }

// ---------------------------------------------------------------------------------------------------------------------
VkPhysicalDevice Hardware::ChooseGPU(Instance& instance)
{ 
    VkPhysicalDevice            chosenPhysicalDevice    = VK_NULL_HANDLE;
    uint32_t                    uDeviceCount;
    VkResult                    result;
    vector<VkPhysicalDevice>    vPhysicalDevices;
    VkPhysicalDeviceFeatures2   pFeatures;


    result = vkEnumeratePhysicalDevices(instance.GetInstance(), &uDeviceCount, NULL);
    if (result != VK_SUCCESS) {
        AB_LOG(Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!! Error code is: %d", result);
        throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
    }
    vPhysicalDevices.resize(uDeviceCount);

    result = vkEnumeratePhysicalDevices(instance.GetInstance(), &uDeviceCount, &vPhysicalDevices[0]);
    if (result != VK_SUCCESS) {
        AB_LOG(Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!! Error code is: %d", result);
        throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
    }



    VkPhysicalDeviceProperties                          deviceProperties;
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR       rayTracingPipelineFeatures;
    VkPhysicalDeviceAccelerationStructureFeaturesKHR    accelStructFeatures;
    VkPhysicalDeviceBufferDeviceAddressFeatures         bufferAddressFeatures;
    VkPhysicalDeviceFeatures2                           deviceFeatures2;

    for (const auto& pDevice : vPhysicalDevices) 
    {
        vkGetPhysicalDeviceProperties(pDevice, &deviceProperties);

        if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            continue;
        }

        memset(static_cast<void*>(&rayTracingPipelineFeatures), 0, sizeof(rayTracingPipelineFeatures));
        rayTracingPipelineFeatures.sType    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
        rayTracingPipelineFeatures.pNext    = NULL;

        memset(static_cast<void*>(&accelStructFeatures), 0, sizeof(accelStructFeatures));
        accelStructFeatures.sType           = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        accelStructFeatures.pNext           = &rayTracingPipelineFeatures;

        memset(static_cast<void*>(&bufferAddressFeatures), 0, sizeof(bufferAddressFeatures));
        bufferAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
        bufferAddressFeatures.pNext = &accelStructFeatures;

        memset(static_cast<void*>(&deviceFeatures2), 0, sizeof(deviceFeatures2));
        deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        deviceFeatures2.pNext = &bufferAddressFeatures;

        vkGetPhysicalDeviceFeatures2(pDevice, &deviceFeatures2);

        if (rayTracingPipelineFeatures.rayTracingPipeline &&
            accelStructFeatures.accelerationStructure &&
            bufferAddressFeatures.bufferDeviceAddress) 
        {
            chosenPhysicalDevice = pDevice;
            break;
        }
    }

    if (chosenPhysicalDevice == VK_NULL_HANDLE) {
        AB_LOG(Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!! Error code is: %d", result);
        throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
    }

    return chosenPhysicalDevice;
}

} // !Core::Voxels
