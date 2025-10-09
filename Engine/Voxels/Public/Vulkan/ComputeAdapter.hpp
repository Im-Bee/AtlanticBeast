#ifndef AB_COMPUTE_DEVICE_ADAPTER_H
#define AB_COMPUTE_DEVICE_ADAPTER_H

#include "Vulkan/WrapperHardware.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace Voxels
{

class ComputeAdapter : public WrapperAdapter
{
public:

    BEAST_VOXEL_API ComputeAdapter(::std::shared_ptr<const WrapperHardware> gpu);

    ~ComputeAdapter() = default;

private:

    uint32_t FindQueueFamilyIndex(const ::std::shared_ptr<const WrapperHardware>& gpu);

    VkDevice CreateDeviceAdapter(const ::std::shared_ptr<const WrapperHardware>& gpu, uint32_t uQueueIndex);

    VkQueue CreateQueue(VkDevice dv, uint32_t uQueueIndex);

private:

    ::std::shared_ptr<const WrapperHardware> m_pGPU = nullptr;

};

} //!Voxels
#endif // !AB_COMPUTE_DEVICE_ADAPTER_H
