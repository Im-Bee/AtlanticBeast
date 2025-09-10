#ifndef AB_HARDWARE_H
#define AB_HARDWARE_H

#include "Instance.hpp"

namespace Voxels
{

class BEAST_VOXEL_API Hardware
{
public:

    Hardware(::std::shared_ptr<Instance>& instance);

    ~Hardware();

public:
    
    VkPhysicalDevice GetPhysicalDevice()
    { return m_DeviceHandle; }

private:
    
    VkPhysicalDevice ChooseGPU(::std::shared_ptr<Instance>& instance);

private:

    VkPhysicalDevice m_DeviceHandle = VK_NULL_HANDLE;

};

} // !Voxels

#endif // !AB_HARDWARE_H
