#ifndef AB_DEVICE_ADAPTER_H
#define AB_DEVICE_ADAPTER_H

#include "Hardware.hpp"

namespace Voxels
{

class BEAST_VOXEL_API DeviceAdapter
{
public:

    DeviceAdapter(::std::shared_ptr<const Hardware> gpu);

    ~DeviceAdapter();
    
public:

    VkDevice GetAdapterHandle() const
    { return m_Device; }

private:

    VkDevice CreateDeviceAdapter(::std::shared_ptr<const Hardware>& gpu);

    uint32_t FindQueueFamilyIndex(::std::shared_ptr<const Hardware>& gpu);

private:

    ::std::shared_ptr<const Hardware> m_pHardware = nullptr;

    VkDevice m_Device = VK_NULL_HANDLE;
    
};

} // !Voxels

#endif // !AB_DEVICE_ADAPTER_H

