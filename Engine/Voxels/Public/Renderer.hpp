#ifndef AB_RENDERER_H
#define AB_RENDERER_H

#include "ErrorHandling.hpp"
#include "Instance.hpp"
#include "Hardware.hpp"
#include "DeviceAdapter.hpp"
#include "SwapChain.hpp"
#include "Pipeline.hpp"

namespace Voxels
{

class BEAST_VOXEL_API Renderer
{
public:

    Renderer()
        : m_pInstance(nullptr)
        , m_pHardware(nullptr)
        , m_pDeviceAdapter(nullptr)
        , m_pWindowDesc(nullptr)
        , m_pSwapChain(nullptr)
        , m_pPipeline(nullptr)
        , m_pVoxelGrid(nullptr)
        , m_CommandPool(VK_NULL_HANDLE)
        , m_CommandBuffer(VK_NULL_HANDLE)
    { }

    ~Renderer() 
    {
		this->Destroy();
    }

public:

    void Initialize(::std::shared_ptr<const WindowDesc> wd);

    void Update();

    void Render();

    void Destroy();

private:

    VkCommandPool CreateCommandPool(::std::shared_ptr<const DeviceAdapter> da, uint32_t uQueueFamily);

    VkCommandBuffer CreateCommandBuffer(::std::shared_ptr<const DeviceAdapter> da, VkCommandPool cmdPool);

private:

    ::std::shared_ptr<Instance>             m_pInstance         = nullptr;
    ::std::shared_ptr<Hardware>             m_pHardware         = nullptr;
    ::std::shared_ptr<DeviceAdapter>        m_pDeviceAdapter    = nullptr;
    ::std::shared_ptr<const WindowDesc>     m_pWindowDesc       = nullptr;
    ::std::shared_ptr<Swapchain>            m_pSwapChain        = nullptr;
    ::std::shared_ptr<Pipeline>             m_pPipeline         = nullptr;
    ::std::shared_ptr<VoxelGrid>            m_pVoxelGrid        = nullptr;

    VkCommandPool       m_CommandPool       = VK_NULL_HANDLE;
    VkCommandBuffer     m_CommandBuffer     = VK_NULL_HANDLE;

};

} // !Voxels

#endif // !AB_RENDERER_H
