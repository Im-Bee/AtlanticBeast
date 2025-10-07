#ifndef AB_DEVICE_ADAPTER_H
#define AB_DEVICE_ADAPTER_H

#include "RTXHardware.hpp"
#include "Vulkan/WrapperHardware.hpp"

namespace Voxels
{

class RTXDeviceAdapter
{
public:

    BEAST_VOXEL_API RTXDeviceAdapter(::std::shared_ptr<const WrapperHardware> gpu);

    BEAST_VOXEL_API ~RTXDeviceAdapter();
    
public:

    uint32_t GetQueueFamilyIndex() const
    { return m_uQueueFamily; }

    VkDevice GetAdapterHandle() const
    { return m_Device; }

    VkQueue GetQueueHandle() const
    { return m_Queue; }

private:

    uint32_t FindQueueFamilyIndex(::std::shared_ptr<const WrapperHardware>& gpu);

    VkDevice CreateDeviceAdapter(::std::shared_ptr<const WrapperHardware>& gpu, uint32_t uQueueIndex);

    VkQueue CreateQueue(VkDevice dv, uint32_t uQueueIndex);

private:

    ::std::shared_ptr<const WrapperHardware> m_pHardware = nullptr;

    uint32_t m_uQueueFamily = 0;

    VkDevice    m_Device    = VK_NULL_HANDLE;
    VkQueue     m_Queue     = VK_NULL_HANDLE;
    
};

} // !Voxels

#endif // !AB_DEVICE_ADAPTER_H

