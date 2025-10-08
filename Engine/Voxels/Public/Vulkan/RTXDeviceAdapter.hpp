#ifndef AB_DEVICE_ADAPTER_H
#define AB_DEVICE_ADAPTER_H

#include "Vulkan/WrapperHardware.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace Voxels
{

class RTXDeviceAdapter : public WrapperAdapter
{
public:

    BEAST_VOXEL_API RTXDeviceAdapter(::std::shared_ptr<const WrapperHardware> gpu);

    BEAST_VOXEL_API ~RTXDeviceAdapter() = default;

private:

    uint32_t FindQueueFamilyIndex(const ::std::shared_ptr<const WrapperHardware>& gpu);

    VkDevice CreateDeviceAdapter(const ::std::shared_ptr<const WrapperHardware>& gpu, uint32_t uQueueIndex);

    VkQueue CreateQueue(VkDevice dv, uint32_t uQueueIndex);

private:

    ::std::shared_ptr<const WrapperHardware> m_pGPU = nullptr;

};

} // !Voxels
#endif // !AB_DEVICE_ADAPTER_H
