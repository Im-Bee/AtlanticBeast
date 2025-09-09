#include "Hardware.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>

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
    VkPhysicalDevice            chosenPhysicalDevice;
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



    for (const auto& pDevice : vPhysicalDevices) 
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(pDevice, &deviceProperties);

        if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            continue;
        }

        VkPhysicalDeviceFeatures2 deviceFeatures2 = { };
        deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;

        VkPhysicalDeviceRayTracingPipelineFeaturesKHR rayTracingPipelineFeatures = { };
        rayTracingPipelineFeatures.sType    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
        deviceFeatures2.pNext               = &rayTracingPipelineFeatures;

        VkPhysicalDeviceAccelerationStructureFeaturesKHR accelStructFeatures = { };
        accelStructFeatures.sType           = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        rayTracingPipelineFeatures.pNext    = &accelStructFeatures;

        VkPhysicalDeviceBufferDeviceAddressFeatures bufferAddressFeatures = { };
        bufferAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
        accelStructFeatures.pNext   = &bufferAddressFeatures;

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
