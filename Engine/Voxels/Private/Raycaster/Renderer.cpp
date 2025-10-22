#include "Raycaster/Renderer.hpp"

#include "Core.h"
#include "Raycaster/VoxelFrameResources.hpp"
#include "Raycaster/VoxelGrid.hpp"
#include "Vulkan/ComputeAdapter.hpp"
#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/GPUStreamBuffer.hpp"
#include "Vulkan/MinimalHardware.hpp"
#include "Vulkan/WrapperAdapter.hpp"
#include "Math/Math.hpp"

namespace Voxels
{

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Initialize(::std::shared_ptr<const WindowDesc> wd,
                          ::std::shared_ptr<VoxelGrid> vg) 
{
    m_pInstance         = make_shared<Instance>();
    m_pHardware         = make_shared<MinimalHardware>(m_pInstance);
    m_pDeviceAdapter    = make_shared<ComputeAdapter>(dynamic_pointer_cast<Hardware>(m_pHardware));
    m_pWindowDesc       = wd;
    m_pVoxelGrid        = vg;
    m_pPipeline         = make_shared<VoxelPipeline>(dynamic_pointer_cast<Hardware>(m_pHardware), 
                                                     dynamic_pointer_cast<Adapter>(m_pDeviceAdapter));

    m_CommandPool = CreateCommandPool(dynamic_pointer_cast<Adapter>(m_pDeviceAdapter), 
                                      m_pDeviceAdapter->GetQueueFamilyIndex());

    // Recreating swap chain also creates frame resources and initializes swap chain
    RecreateSwapChain();

    if (m_pCamera == nullptr) {
        m_pCamera = make_shared<Camera>();
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Update()
{
    m_pPipeline->UploadOnStreamBuffer(&m_pVoxelGrid->GetGrid()[0], 
                                      m_vFrames[m_uCurrentFrame].VoxelBuffer,
                                      VoxelPipeline::ShaderResource::VoxelGrid);

    Vec3 rot = m_pCamera->GetRotation();
    Vec3 rotVec = Normalize(RotateY(RotateX(Vec3 { 0.f, 0.f, 1.f }, rot.x), rot.y));
    Vec3 cameraRight = Normalize(Cross(rotVec, Vec3 { 0.f, 1.f, 0.f }));
    Vec3 cameraUp = Cross(cameraRight, rotVec);

    m_pPipeline->LoadPushConstants(m_pCamera->GetFov() * AB_DEG_TO_RAD,
                                   m_pCamera->GetPosition(), 
                                   rotVec, 
                                   cameraRight,
                                   cameraUp,
                                   m_pVoxelGrid->GetGridWidth());
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Render()
{
    VkResult result;
    VkDevice device = m_pDeviceAdapter->GetAdapterHandle();
    uint32_t uImageIndex;
    FrameResources& frame = m_vFrames[m_uCurrentFrame];

    THROW_IF_FAILED(vkWaitForFences(device, 1, &frame.InFlightFence, VK_TRUE, UINT64_MAX));
    THROW_IF_FAILED(vkResetFences(device, 1, &frame.InFlightFence));

    result = vkAcquireNextImageKHR(device, 
                                   m_pSwapChain->GetSwapChainHandle(), 
                                   UINT64_MAX,
                                   frame.ImageAvailable,
                                   VK_NULL_HANDLE,
                                   &uImageIndex);

    if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR) {
        RecreateSwapChain();
        return;
    }

    m_pPipeline->LoadImage(m_pSwapChain->GetImage(uImageIndex));

    RecordCommands(frame.CommandBuffer,
                   m_pPipeline,
                   uImageIndex);

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo submitInfo = { };
    submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = &frame.ImageAvailable;
    submitInfo.pWaitDstStageMask    = waitStages;
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &frame.CommandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = &frame.RenderFinished;

    THROW_IF_FAILED(vkQueueSubmit(m_pDeviceAdapter->GetQueueHandle(), 1, &submitInfo, frame.InFlightFence));

    VkSwapchainKHR swapchain = m_pSwapChain->GetSwapChainHandle();
    VkPresentInfoKHR presentInfo = { };
    presentInfo.sType                   = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount      = 1;
    presentInfo.pWaitSemaphores         = &frame.RenderFinished;
    presentInfo.swapchainCount          = 1;
    presentInfo.pSwapchains             = &swapchain;
    presentInfo.pImageIndices           = &uImageIndex;

    result = vkQueuePresentKHR(m_pDeviceAdapter->GetQueueHandle(), &presentInfo);
    if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR) {
        RecreateSwapChain();
        return;
    }

    m_uCurrentFrame = (++m_uCurrentFrame) % MAX_FRAMES_IN_FLIGHT;
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Destroy()
{
    if (m_pDeviceAdapter != nullptr) {
        vkDeviceWaitIdle(m_pDeviceAdapter->GetAdapterHandle());
    }
    if (m_pDeviceAdapter != nullptr)
    {
        for (size_t i = 0; i < m_vFrames.size(); ++i)
        {
            vkDestroySemaphore(m_pDeviceAdapter->GetAdapterHandle(), m_vFrames[i].RenderFinished, nullptr);
            vkDestroySemaphore(m_pDeviceAdapter->GetAdapterHandle(), m_vFrames[i].ImageAvailable, nullptr);
            vkDestroyFence(m_pDeviceAdapter->GetAdapterHandle(), m_vFrames[i].InFlightFence, nullptr);
            vkFreeCommandBuffers(m_pDeviceAdapter->GetAdapterHandle(), m_CommandPool, 1, &m_vFrames[i].CommandBuffer);
            m_vFrames[i].VoxelBuffer.~GPUStreamBuffer();
        }
    }
    if (m_CommandPool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(m_pDeviceAdapter->GetAdapterHandle(), m_CommandPool, nullptr);
        m_CommandPool = VK_NULL_HANDLE;
    }
    if (m_pPipeline != nullptr) {
        m_pPipeline = nullptr;
    }
    if (m_pSwapChain != nullptr) {
        m_pSwapChain = nullptr;
    }
    if (m_pDeviceAdapter != nullptr) {
        m_pDeviceAdapter = nullptr;
    }
    if (m_pHardware != nullptr) {
        m_pHardware = nullptr;
    }
    if (m_pInstance != nullptr) {
        m_pInstance = nullptr;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
VkCommandPool Renderer::CreateCommandPool(shared_ptr<const Adapter> da, uint32_t uQueueFamily)
{
    VkCommandPool cmdPool;

    VkCommandPoolCreateInfo cmdPoolInfo = { };
    cmdPoolInfo.sType               = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.flags               = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    cmdPoolInfo.queueFamilyIndex    = uQueueFamily;

    THROW_IF_FAILED(vkCreateCommandPool(da->GetAdapterHandle(), &cmdPoolInfo, NULL, &cmdPool));

    return cmdPool;
}
 
// ---------------------------------------------------------------------------------------------------------------------
VkCommandBuffer Renderer::CreateCommandBuffer(::std::shared_ptr<const Adapter> da, VkCommandPool cmdPool)
{
    VkCommandBuffer             cmdBuffer;
    VkCommandBufferAllocateInfo allocInfo;

    allocInfo.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext                 = NULL;
    allocInfo.commandPool           = cmdPool;
    allocInfo.level                 = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount    = 1;

    THROW_IF_FAILED(vkAllocateCommandBuffers(m_pDeviceAdapter->GetAdapterHandle(), &allocInfo, &cmdBuffer));
    THROW_IF_FAILED(vkResetCommandBuffer(cmdBuffer, 0));

    return cmdBuffer;
}

// ---------------------------------------------------------------------------------------------------------------------
vector<VoxelFrameResources> Renderer::CreateFrameResources(const ::std::shared_ptr<const Adapter>& da,
                                                           const ::std::shared_ptr<VoxelPipeline>& pipeline,
                                                           const ::std::shared_ptr<const VoxelGrid>& vg,
                                                           VkCommandPool cmdPool,
                                                           size_t uFrames)
{
    VkDevice device = da->GetAdapterHandle();
    vector<VoxelFrameResources> result(uFrames);

    VkSemaphoreCreateInfo semaphoreInfo = { };
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = { };
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < result.size(); ++i) 
    {
        if (vkCreateSemaphore(device, &semaphoreInfo, NULL, &result[i].ImageAvailable) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, NULL, &result[i].RenderFinished) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, NULL, &result[i].InFlightFence)          != VK_SUCCESS)
        {
            throw AB_EXCEPT("Failed to create frame resources!");
        }

        result[i].CommandBuffer = CreateCommandBuffer(da, cmdPool);
        result[i].VoxelBuffer   = std::move(pipeline->ReserveBuffer(vg->GetAmountOfVoxels() * sizeof(Voxel)));
    }

    return result;
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::RecordCommands(VkCommandBuffer& cmdBuff,
                              const shared_ptr<VoxelPipeline>& pipeline,
                              uint32_t uImageIndex)
{
    vkResetCommandBuffer(cmdBuff, 0);

    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = NULL;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    beginInfo.pInheritanceInfo = NULL;

    THROW_IF_FAILED(vkBeginCommandBuffer(cmdBuff, &beginInfo));
    vkCmdBindPipeline(cmdBuff, VK_PIPELINE_BIND_POINT_COMPUTE, m_pPipeline->GetPipelineHandle());

    VkImageMemoryBarrier barrier = { };
    barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.oldLayout           = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    barrier.newLayout           = VK_IMAGE_LAYOUT_GENERAL;
    barrier.srcAccessMask       = 0;
    barrier.dstAccessMask       = VK_ACCESS_SHADER_WRITE_BIT;
    barrier.image               = m_pSwapChain->GetImage(uImageIndex);
    barrier.subresourceRange    = VkImageSubresourceRange { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    vkCmdPipelineBarrier(cmdBuff,
                         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                         VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                         0,
                         0, NULL,
                         0, NULL,
                         1, &barrier);

    RecordVoxelesCommands(cmdBuff, pipeline);

    VkImageMemoryBarrier presentBarrier = { };
    presentBarrier.sType            = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    presentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    presentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    presentBarrier.oldLayout        = VK_IMAGE_LAYOUT_GENERAL;
    presentBarrier.newLayout        = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    presentBarrier.srcAccessMask    = VK_ACCESS_SHADER_WRITE_BIT;
    presentBarrier.dstAccessMask    = 0;
    presentBarrier.image            = m_pSwapChain->GetImage(uImageIndex);
    presentBarrier.subresourceRange = VkImageSubresourceRange { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    vkCmdPipelineBarrier(cmdBuff,
                         VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                         0,
                         0, NULL,
                         0, NULL,
                         1, &presentBarrier);

    THROW_IF_FAILED(vkEndCommandBuffer(cmdBuff));
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::RecordVoxelesCommands(VkCommandBuffer& cmdBuffer, const ::std::shared_ptr<VoxelPipeline>& pipeline)
{
    vkCmdBindDescriptorSets(cmdBuffer,
                            VK_PIPELINE_BIND_POINT_COMPUTE,
                            pipeline->GetLayoutHandle(),
                            0, 1,
                            &pipeline->GetDescrpitorSet(),
                            0, NULL);

    vkCmdPushConstants(cmdBuffer,
                       pipeline->GetLayoutHandle(),
                       VK_SHADER_STAGE_COMPUTE_BIT,
                       0,
                       sizeof(VoxelPushConstants),
                       &pipeline->GetPushConstants());

    VkBufferMemoryBarrier voxelBufferBarrier = { };
    voxelBufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    voxelBufferBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
    voxelBufferBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    voxelBufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    voxelBufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    voxelBufferBarrier.buffer = m_vFrames[m_uCurrentFrame].VoxelBuffer.GetBufferHandle();
    voxelBufferBarrier.offset = 0;
    voxelBufferBarrier.size = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier(cmdBuffer,
                         VK_PIPELINE_STAGE_HOST_BIT,
                         VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                         0,
                         0, NULL,
                         1, &voxelBufferBarrier,
                         0, NULL);

    const uint32_t groupCountX = (m_pWindowDesc->Width + 15) / 16;
    const uint32_t groupCountY = (m_pWindowDesc->Height + 15) / 16;
    vkCmdDispatch(cmdBuffer, groupCountX, groupCountY, 1);
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::RecreateSwapChain()
{
    AB_LOG(Core::Debug::Info, L"Recreating swapchain");
    if (m_pWindowDesc->IsAlive == false) {
        return;
    }

    for (size_t i = 0; i < m_vFrames.size(); ++i)
    {
        vkWaitForFences(m_pDeviceAdapter->GetAdapterHandle(), 1, &m_vFrames[i].InFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(m_pDeviceAdapter->GetAdapterHandle(), 1, &m_vFrames[i].InFlightFence);
        vkResetCommandBuffer(m_vFrames[i].CommandBuffer, 0);

        vkDestroySemaphore(m_pDeviceAdapter->GetAdapterHandle(), m_vFrames[i].RenderFinished, nullptr);
        vkDestroySemaphore(m_pDeviceAdapter->GetAdapterHandle(), m_vFrames[i].ImageAvailable, nullptr);
        vkDestroyFence(m_pDeviceAdapter->GetAdapterHandle(), m_vFrames[i].InFlightFence, nullptr);
        vkFreeCommandBuffers(m_pDeviceAdapter->GetAdapterHandle(), m_CommandPool, 1, &m_vFrames[i].CommandBuffer);
    }

    m_pSwapChain = nullptr;
    m_pSwapChain = make_unique<Swapchain>(m_pInstance, 
                                          dynamic_pointer_cast<Hardware>(m_pHardware), 
                                          dynamic_pointer_cast<Adapter>(m_pDeviceAdapter),
                                          m_pWindowDesc);

    m_vFrames = std::move(CreateFrameResources(m_pDeviceAdapter,
                                               m_pPipeline,
                                               m_pVoxelGrid,
                                               m_CommandPool,
                                               MAX_FRAMES_IN_FLIGHT));
    m_uCurrentFrame = 0;
    AB_LOG(Core::Debug::Info, L"Swapchain recreated");
}

} // !Voxels
