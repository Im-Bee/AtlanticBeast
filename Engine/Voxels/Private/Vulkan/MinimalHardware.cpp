#include "Vulkan/MinimalHardware.hpp"
#include "Core.h"

namespace Voxels
{

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
MinimalHardware::MinimalHardware(shared_ptr<const Instance> pInstance)
    : Hardware(ChooseGPU(pInstance))
    , m_pInstance(pInstance)
{ }

// ---------------------------------------------------------------------------------------------------------------------
VkPhysicalDevice MinimalHardware::ChooseGPU(const ::std::shared_ptr<const Instance>& pInstance)
{ 
    VkPhysicalDevice            chosenPhysicalDevice    = VK_NULL_HANDLE;
    vector<VkPhysicalDevice>    vPhysicalDevices        = GetPhysicalDevices(pInstance->GetInstance());

    VkPhysicalDeviceProperties  deviceProperties;

    for (const auto& pDevice : vPhysicalDevices) 
    {
        vkGetPhysicalDeviceProperties(pDevice, &deviceProperties);
        
        if (chosenPhysicalDevice == VK_NULL_HANDLE && pDevice != VK_NULL_HANDLE) {
            chosenPhysicalDevice = pDevice;
        }

        if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            continue;
        }

        chosenPhysicalDevice = pDevice;
        break;
    }

    if (chosenPhysicalDevice == VK_NULL_HANDLE) {
        AB_LOG(Core::Debug::Error, L"Ohh nooo... Couldn't choose a valid physical gpu!!!");
        throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
    }

    return chosenPhysicalDevice;
}

} // !Voxels
