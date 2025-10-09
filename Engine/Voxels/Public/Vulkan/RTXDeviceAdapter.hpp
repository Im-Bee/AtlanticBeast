#ifndef AB_DEVICE_ADAPTER_H
#define AB_DEVICE_ADAPTER_H

#include "Vulkan/IAdapter.hpp"
#include "Vulkan/WrapperHardware.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace Voxels
{

class RTXDeviceAdapter : public Adapter
                       , public IAdapter<RTXDeviceAdapter>
{
public:

    BEAST_VOXEL_API RTXDeviceAdapter(::std::shared_ptr<const Hardware> gpu);

    BEAST_VOXEL_API ~RTXDeviceAdapter() = default;

public:

    const ::std::vector<const char*> GetExtensionsImpl() const;

    void* GetFeaturesImpl() const;

private:

    ::std::shared_ptr<const Hardware> m_pGPU = nullptr;

};

} // !Voxels
#endif // !AB_DEVICE_ADAPTER_H
