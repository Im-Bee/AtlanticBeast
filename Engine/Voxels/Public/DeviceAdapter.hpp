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

    uint32_t GetQueueFamilyIndex() const
    { return m_uQueueFamily; }

    VkDevice GetAdapterHandle() const
    { return m_Device; }

    VkQueue GetQueueHandle() const
    { return m_Queue; }

private:

    uint32_t FindQueueFamilyIndex(::std::shared_ptr<const Hardware>& gpu);

    VkDevice CreateDeviceAdapter(::std::shared_ptr<const Hardware>& gpu, uint32_t uQueueIndex);

    VkQueue CreateQueue(VkDevice dv, uint32_t uQueueIndex);

private:

    ::std::shared_ptr<const Hardware> m_pHardware = nullptr;

    uint32_t m_uQueueFamily = 0;

    VkDevice    m_Device    = VK_NULL_HANDLE;
    VkQueue     m_Queue     = VK_NULL_HANDLE;
    
};

} // !Voxels

#endif // !AB_DEVICE_ADAPTER_H

