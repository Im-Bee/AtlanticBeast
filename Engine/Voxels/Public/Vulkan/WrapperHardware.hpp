#ifndef AB_WRAPPER_HARDWARE_H
#define AB_WRAPPER_HARDWARE_H

namespace Voxels
{

class WrapperHardware
{
public:

    WrapperHardware() = delete;

    explicit WrapperHardware(VkPhysicalDevice physicalDevice)
        : m_PhysicalDevice(physicalDevice)
    { }

    ~WrapperHardware() = default;

public: 

    VkPhysicalDevice GetPhysicalDevice() const
    { return m_PhysicalDevice; }

private:

    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;

};

} // !Voxels

#endif // !AB_WRAPPER_HARDWARE_H
