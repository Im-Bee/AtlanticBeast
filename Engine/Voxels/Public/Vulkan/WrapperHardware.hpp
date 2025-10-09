#ifndef AB_WRAPPER_HARDWARE_H
#define AB_WRAPPER_HARDWARE_H

namespace Voxels
{

class Hardware
{
public:

    Hardware() = delete;

    explicit Hardware(VkPhysicalDevice physicalDevice)
        : m_PhysicalDevice(physicalDevice)
    { }

    Hardware(Hardware&&) noexcept = default;
    Hardware(const Hardware&) = delete;

    ~Hardware() = default;

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
