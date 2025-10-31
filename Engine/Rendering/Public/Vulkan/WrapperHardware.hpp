#ifndef AB_WRAPPER_HARDWARE_H
#define AB_WRAPPER_HARDWARE_H

#include "Voxels.hpp"

namespace Voxels
{

class HardwareWrapper
{
public:

    HardwareWrapper() = delete;

    explicit HardwareWrapper(VkPhysicalDevice physicalDevice)
        : m_PhysicalDevice(physicalDevice)
    { 
        AB_LOG(Core::Debug::Info, L"Initializing hardware");
    }

    HardwareWrapper(HardwareWrapper&&) noexcept = default;
    HardwareWrapper(const HardwareWrapper&) = delete;

    ~HardwareWrapper() 
    {
        AB_LOG(Core::Debug::Info, L"Destroying hardware");
    }

public: 

    VkPhysicalDevice GetPhysicalDevice() const
    { return m_PhysicalDevice; }

protected:

    ::std::vector<VkPhysicalDevice> GetPhysicalDevices(VkInstance Instance);

private:

    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;

};

} // !Voxels

#endif // !AB_WRAPPER_HARDWARE_H
