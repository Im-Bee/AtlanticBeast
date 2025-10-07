#ifndef AB_MINIMAL_HARDWARE_H
#define AB_MINIMAL_HARDWARE_H

#include "WrapperHardware.hpp"
#include "Instance.hpp"

namespace Voxels
{

class MinimalHardware : public WrapperHardware
{
public: 

    BEAST_VOXEL_API MinimalHardware(::std::shared_ptr<const Instance> pInstance);

    ~MinimalHardware() = default; 

private:

    VkPhysicalDevice ChooseGPU(const ::std::shared_ptr<const Instance>& pInstance);

private:

    ::std::shared_ptr<const Instance> m_pInstance = nullptr;

};

} // !Voxels

#endif // !AB_MINIMAL_HARDWARE_H
