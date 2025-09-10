#ifndef AB_DEVICE_ADAPTER_H
#define AB_DEVICE_ADAPTER_H

#include "Hardware.hpp"

namespace Voxels
{

class DeviceAdapter 
{
public:

    DeviceAdapter(Hardware& gpu);

    ~DeviceAdapter();

private:

    VkDevice CreateDeviceAdapter(Hardware& gpu);

    uint32_t FindQueueFamilyIndex(Hardware& gpu);

private:

    VkDevice m_Device = VK_NULL_HANDLE;
    
};

} // !Voxels

#endif // !AB_DEVICE_ADAPTER_H

