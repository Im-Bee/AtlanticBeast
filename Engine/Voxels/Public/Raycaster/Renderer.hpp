#ifndef AB_RENDERER_H
#define AB_RENDERER_H

#include "Vulkan/ComputeAdapter.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/MinimalHardware.hpp"
#include "Vulkan/RTXDeviceAdapter.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Raycaster/VoxelPipeline.hpp"
#include "Raycaster/VoxelFrameResources.hpp"

#include "Primitives/Camera.hpp"
#include <vulkan/vulkan_enums.hpp>

namespace Voxels
{

class Renderer
{
public:

    BEAST_VOXEL_API Renderer()
        : m_pInstance(nullptr)
        , m_pHardware(nullptr)
        , m_pDeviceAdapter(nullptr)
        , m_pWindowDesc(nullptr)
        , m_pSwapChain()
        , m_pPipeline(nullptr)
        , m_pVoxelGrid(nullptr)
        , m_CommandPool(VK_NULL_HANDLE)
        , m_uCurrentFrame(0)
        , m_vFrames()
    { }

    BEAST_VOXEL_API ~Renderer()
    { this->Destroy(); }

public:

    void SetCurrentCamera(::std::shared_ptr<Camera> camera)
    { m_pCamera = camera; }

public:

    ::std::shared_ptr<Camera>& GetCurrentCamera()
    {
        if (m_pCamera == nullptr) {
            AB_LOG(Core::Debug::Warning, L"Renderer doesn't have a camera. Recreating camera for getter.");
            m_pCamera = ::std::make_shared<Camera>();
        }

        return m_pCamera;
    }

    ::std::shared_ptr<VoxelGrid>& GetGrid()
    { return m_pVoxelGrid; }

public:

    BEAST_VOXEL_API void Initialize(::std::shared_ptr<const WindowDesc> wd,
                                    ::std::shared_ptr<VoxelGrid> vg = ::std::make_shared<VoxelGrid>());

    BEAST_VOXEL_API void Update();

    BEAST_VOXEL_API void Render();

    BEAST_VOXEL_API void Destroy();

private:

    VkCommandPool CreateCommandPool(::std::shared_ptr<const WrapperAdapter> da, uint32_t uQueueFamily);

    VkCommandBuffer CreateCommandBuffer(::std::shared_ptr<const WrapperAdapter> da, VkCommandPool cmdPool);

    ::std::vector<VoxelFrameResources> CreateFrameResources(const ::std::shared_ptr<const WrapperAdapter>& da,
                                                            const ::std::shared_ptr<VoxelPipeline>& pipeline,
                                                            const ::std::shared_ptr<const VoxelGrid>& vg,
                                                            VkCommandPool cmdPool,
                                                            size_t uFrames);

    void RecordCommands(VkCommandBuffer& cmdBuff,
                        const ::std::shared_ptr<VoxelPipeline>& pipeline, 
                        uint32_t uImageIndex);

    void RecordVoxelesCommands(VkCommandBuffer& cmdBuffer, const ::std::shared_ptr<VoxelPipeline>& pipeline);

    void RecreateSwapChain();

private:

    ::std::shared_ptr<Instance>             m_pInstance         = nullptr;
    ::std::shared_ptr<MinimalHardware>      m_pHardware         = nullptr;
    ::std::shared_ptr<ComputeAdapter>       m_pDeviceAdapter    = nullptr;
    ::std::unique_ptr<Swapchain>            m_pSwapChain        = nullptr;
    ::std::shared_ptr<const WindowDesc>     m_pWindowDesc       = nullptr;
    ::std::shared_ptr<VoxelPipeline>        m_pPipeline         = nullptr;
    ::std::shared_ptr<VoxelGrid>            m_pVoxelGrid        = nullptr;


    VkCommandPool m_CommandPool = VK_NULL_HANDLE;

    ::std::shared_ptr<Camera> m_pCamera = nullptr;

    size_t m_uCurrentFrame;
    ::std::vector<VoxelFrameResources> m_vFrames;

};

} // !Voxels

#endif // !AB_RENDERER_H
