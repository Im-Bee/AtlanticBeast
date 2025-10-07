#ifndef AB_RTX_HARDWARE_H
#define AB_RTX_HARDWARE_H

#include "Instance.hpp"
#include "WrapperHardware.hpp"

namespace Voxels
{

/**
* Hardware wrapper that selects a GPU with features:
* - rayTracingPipeline
* - accelerationStructure
* - bufferDeviceAddress
**/
class RTXHardware : public WrapperHardware
{
public:

    BEAST_VOXEL_API RTXHardware(::std::shared_ptr<const Instance> pInstance);

    BEAST_VOXEL_API ~RTXHardware() = default;

private:
    
    VkPhysicalDevice ChooseGPU(const ::std::shared_ptr<const Instance>& pInstance);

private:

    ::std::shared_ptr<const Instance> m_pInstance = nullptr;

};

} // !Voxels

#endif // !AB_RTX_HARDWARE_H
