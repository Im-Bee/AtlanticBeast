#ifndef AB_DEVICE_ADAPTER_H
#define AB_DEVICE_ADAPTER_H

#include "Vulkan/IAdapter.hpp"
#include "Vulkan/WrapperHardware.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace Voxels
{

class RTXDeviceAdapter : public AdapterWrapper
                       , public IAdapter<RTXDeviceAdapter>
{
public:

    RTXDeviceAdapter(::std::shared_ptr<const HardwareWrapper> gpu)
        : AdapterWrapper(gpu,
                         VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT,
                         GetExtensionsImpl(),
                         GetFeaturesImpl())
    { }

public:

    const ::std::vector<const char*> GetExtensionsImpl() const;

    void* GetFeaturesImpl() const;

};

} // !Voxels
#endif // !AB_DEVICE_ADAPTER_H
