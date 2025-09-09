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

private:
    
    VkPhysicalDevice ChooseGPU(Instance& instance);

private:

    VkPhysicalDevice m_DeviceHandle;

};

} // !Core::Voxels

#endif // !AB_HARDWARE_H
