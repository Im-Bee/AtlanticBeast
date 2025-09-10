#ifndef AB_DEVICE_ADAPTER_H
#define AB_DEVICE_ADAPTER_H

#include "Hardware.hpp"

namespace Voxels
{

class BEAST_API DeviceAdapter 
{
public:

    DeviceAdapter(::std::shared_ptr<Hardware>& gpu);

    ~DeviceAdapter();

private:

    VkDevice CreateDeviceAdapter(::std::shared_ptr<Hardware>& gpu);

    uint32_t FindQueueFamilyIndex(::std::shared_ptr<Hardware>& gpu);

private:

    VkDevice m_Device = VK_NULL_HANDLE;
    
};

} // !Voxels

#endif // !AB_DEVICE_ADAPTER_H

