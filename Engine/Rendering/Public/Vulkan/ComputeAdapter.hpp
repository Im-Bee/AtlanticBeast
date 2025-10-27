#ifndef AB_COMPUTE_DEVICE_ADAPTER_H
#define AB_COMPUTE_DEVICE_ADAPTER_H

#include "Vulkan/IAdapter.hpp"
#include "Vulkan/WrapperHardware.hpp"
#include "Vulkan/WrapperAdapter.hpp"

namespace Voxels
{

class ComputeAdapter : public AdapterWrapper 
                     , public IAdapter<ComputeAdapter>
{
public:

    BEAST_API ComputeAdapter(::std::shared_ptr<const HardwareWrapper> gpu);

    ~ComputeAdapter() = default;

public:

    ComputeAdapter(const ComputeAdapter&) = delete;
    ComputeAdapter(ComputeAdapter&&) = default;

    ComputeAdapter& operator=(const ComputeAdapter&) noexcept = delete;
    ComputeAdapter& operator=(ComputeAdapter&& other) noexcept 
    {
        m_pGPU = std::move(other.m_pGPU);
        return *this;
    }

public:

    const ::std::vector<const char*>& GetExtensionsImpl() const;

    void* GetFeaturesImpl() const;

private:

    ::std::shared_ptr<const HardwareWrapper> m_pGPU = nullptr;

};

} //!Voxels
#endif // !AB_COMPUTE_DEVICE_ADAPTER_H
