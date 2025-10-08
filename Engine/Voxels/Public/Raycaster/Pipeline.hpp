#ifndef AB_PIPELINE_H
#define AB_PIPELINE_H

#include "Vulkan/GPUBuffer.hpp"
#include "Vulkan/GPUStreamBuffer.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/RTXDeviceAdapter.hpp"
#include "Raycaster/PushConstants.hpp"
#include "Raycaster/VoxelGrid.hpp"
#include "Voxels.hpp"

namespace Voxels
{

class Pipeline
{

    friend class Renderer;

public:

    BEAST_VOXEL_API Pipeline(::std::shared_ptr<const WrapperHardware> hw,
                             ::std::shared_ptr<const RTXDeviceAdapter> da);

    BEAST_VOXEL_API ~Pipeline();

public:

    BEAST_VOXEL_API GPUStreamBuffer ReserveGridBuffer(const ::std::shared_ptr<const VoxelGrid>& vg);

    BEAST_VOXEL_API void LoadGrid(const ::std::shared_ptr<const VoxelGrid>& vg, GPUStreamBuffer& outBuffer);

    BEAST_VOXEL_API void LoadImage(VkImage image);

    void LoadPushConstants(float fFov,
                           Vec3 cameraPos, 
                           Vec3 cameraLookDir,
                           Vec3 cameraRight,
                           Vec3 cameraUp)
    {
        m_Vpc.CameraPos = cameraPos;
        m_Vpc.CameraLookDir = cameraLookDir;
        m_Vpc.CameraRight = cameraRight;
        m_Vpc.CameraUp = cameraUp;
        m_Vpc.fFov = fFov;
    }

public:

    VkPipelineLayout GetLayoutHandle() const
    { return m_PipelineLayout; }

    VkPipeline GetPipelineHandle() const
    { return m_ComputePipeline; }

    VkDescriptorSet& GetDescrpitorSet()
    { return m_DescriptorSet; }

    const VoxelPushConstants& GetPushConstants() const
    { return m_Vpc; }

private:

    VkDescriptorSetLayout CreateDescriptorLayout(::std::shared_ptr<const RTXDeviceAdapter>& da);

    VkDescriptorPool CreateDescriptorPool(::std::shared_ptr<const RTXDeviceAdapter>& da);

    VkDescriptorSet CreateDescriptorSet(::std::shared_ptr<const RTXDeviceAdapter>& da,
                                        VkDescriptorPool dp,
                                        VkDescriptorSetLayout dLayout);

    VkPipelineLayout CreatePipelineLayout(::std::shared_ptr<const RTXDeviceAdapter>& da,
                                          VkDescriptorSetLayout descriptorSetLayout);

    VkShaderModule LoadShader(::std::shared_ptr<const RTXDeviceAdapter>& da, const ::std::string& strPath);

    VkPipeline CreateComputePipeline(::std::shared_ptr<const RTXDeviceAdapter>& da, 
                                     VkPipelineLayout pipelineLayout, 
                                     VkShaderModule shaderModule);

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

private:
    
    ::std::shared_ptr<const WrapperHardware>   m_pHardware         = nullptr;
    ::std::shared_ptr<const RTXDeviceAdapter>  m_pDeviceAdapter    = nullptr;
    ::std::shared_ptr<const Swapchain>         m_pSwapChain        = nullptr;

    ::std::shared_ptr<const VoxelGrid> m_VoxelGrid = nullptr;

    VoxelPushConstants m_Vpc;

    VkDescriptorSetLayout   m_DescriptorLayout  = VK_NULL_HANDLE;
    VkDescriptorPool        m_DescriptorPool    = VK_NULL_HANDLE;
    VkDescriptorSet         m_DescriptorSet     = VK_NULL_HANDLE;
    VkPipelineLayout        m_PipelineLayout    = VK_NULL_HANDLE;
    VkShaderModule		    m_ShaderModule      = VK_NULL_HANDLE;
    VkPipeline              m_ComputePipeline   = VK_NULL_HANDLE;

    VkImageView m_ImageView = VK_NULL_HANDLE;
};

} // !Voxels

#endif // !AB_PIPELINE_H

