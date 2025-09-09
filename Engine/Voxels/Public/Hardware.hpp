#ifndef AB_HARDWARE_H
#define AB_HARDWARE_H

#include "Voxels.h"

namespace Core::Voxels
{

class Hardware 
{
public:

    Hardware(VkInstance& instance);

    ~Hardware();

private:
    
    VkPhysicalDevice* ChooseGPU(VkInstance& instance);

private:

    VkPhysicalDevice* m_DeviceHandle;

};

} // !Core::Voxels

#endif // !AB_HARDWARE_H
