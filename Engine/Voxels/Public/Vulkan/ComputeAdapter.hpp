#ifndef AB_COMPUTE_DEVICE_ADAPTER_H
#define AB_COMPUTE_DEVICE_ADAPTER_H

#include "Vulkan/IAdapter.hpp"
#include "Vulkan/WrapperHardware.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace Voxels
{

class ComputeAdapter : public Adapter 
                     , public IAdapter<ComputeAdapter>
{
public:

    BEAST_VOXEL_API ComputeAdapter(::std::shared_ptr<const Hardware> gpu);

    ~ComputeAdapter() = default;

public:

    const ::std::vector<const char*>& GetExtensionsImpl() const;

    void* GetFeaturesImpl() const;

private:

    ::std::shared_ptr<const Hardware> m_pGPU = nullptr;

};

} //!Voxels
#endif // !AB_COMPUTE_DEVICE_ADAPTER_H
