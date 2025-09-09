#ifndef AB_HARDWARE_H
#define AB_HARDWARE_H

#include "Instance.hpp"

namespace Voxels
{

class Hardware 
{
public:

    Hardware(Instance& instance);

    ~Hardware();

public:
    
    VkPhysicalDevice GetPhysicalDevice()
    { return m_DeviceHandle; }

private:
    
    VkPhysicalDevice ChooseGPU(Instance& instance);

private:

    VkPhysicalDevice m_DeviceHandle = VK_NULL_HANDLE;

};

} // !Core::Voxels

#endif // !AB_HARDWARE_H
