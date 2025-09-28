#ifndef AB_HARDWARE_H
#define AB_HARDWARE_H

#include "Instance.hpp"

namespace Voxels
{

class Hardware
{
public:

    BEAST_VOXEL_API Hardware(::std::shared_ptr<const Instance> instance);

    BEAST_VOXEL_API ~Hardware();

public:
    
    VkPhysicalDevice GetPhysicalDevice() const
    { return m_DeviceHandle; }

private:
    
    VkPhysicalDevice ChooseGPU(::std::shared_ptr<const Instance>& instance);

private:

    ::std::shared_ptr<const Instance> m_pInstance = nullptr;

    VkPhysicalDevice m_DeviceHandle = VK_NULL_HANDLE;

};

} // !Voxels

#endif // !AB_HARDWARE_H
