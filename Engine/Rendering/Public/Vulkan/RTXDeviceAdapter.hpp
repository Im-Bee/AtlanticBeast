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

    BEAST_API RTXDeviceAdapter(::std::shared_ptr<const HardwareWrapper> gpu);

    ~RTXDeviceAdapter() = default;

public:

    const ::std::vector<const char*> GetExtensionsImpl() const;

    void* GetFeaturesImpl() const;

private:

    ::std::shared_ptr<const HardwareWrapper> m_pGPU = nullptr;

};

} // !Voxels
#endif // !AB_DEVICE_ADAPTER_H
