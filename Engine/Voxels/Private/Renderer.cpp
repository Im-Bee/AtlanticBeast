#include "Renderer.hpp"

namespace Voxels
{

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Initialize(::std::shared_ptr<const WindowDesc> wd) 
{
    m_pInstance         = make_shared<Instance>();
    m_pHardware         = make_shared<Hardware>(m_pInstance);
    m_pDeviceAdapter    = make_shared<DeviceAdapter>(m_pHardware);
    m_pWindowDesc       = wd;
    m_pSwapChain        = make_shared<Swapchain>(m_pInstance, m_pHardware, m_pDeviceAdapter, wd);
    m_pVoxelGrid        = make_shared<VoxelGrid>();
    m_pPipeline         = make_shared<Pipeline>(m_pHardware, m_pDeviceAdapter);

    m_CommandPool = CreateCommandPool(m_pDeviceAdapter, m_pDeviceAdapter->GetQueueFamilyIndex());
    m_CommandBuffer = CreateCommandBuffer(m_pDeviceAdapter, m_CommandPool);

    m_pPipeline->ReserveGridBuffer(m_pVoxelGrid);
    m_pPipeline->LoadGrid(m_pVoxelGrid);

}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Render()
{
    VkDevice device = m_pDeviceAdapter->GetAdapterHandle();
    static VoxelPushConstants vpc = {
        { 20.0f, 5.0f, -10.0f },
        0,
        { 64, 64, 64 },
        0,
{
    -0.48191875,  5.6898e-17, -0.87621591,  2.675e-18,
    -0.38039632,  0.90084765,  0.20921798,  2.1116e-18,
    0.78933704,  0.43413537, -0.43413537, -4.3817e-18,
    20.00000000, 11.00000000, -10.00000000,  1.00000000
}

    };

    uint32_t imageIndex;
    vkAcquireNextImageKHR(device, m_pSwapChain->GetSwapChainHandle(), UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE, &imageIndex);
    m_pPipeline->LoadImage(m_pSwapChain->GetImage(imageIndex));

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);

    vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_pPipeline->GetPipelineHandle());

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
    barrier.image = m_pSwapChain->GetImage(imageIndex);
    barrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    vkCmdPipelineBarrier(
        m_CommandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    vkCmdBindDescriptorSets(
        m_CommandBuffer,
        VK_PIPELINE_BIND_POINT_COMPUTE,
        m_pPipeline->GetLayoutHandle(),
        0, 1,
        &m_pPipeline->GetDescrpitorSet(),
        0, nullptr
    );
    vkCmdPushConstants(
        m_CommandBuffer,
        m_pPipeline->GetLayoutHandle(),
        VK_SHADER_STAGE_COMPUTE_BIT,
        0,
        sizeof(VoxelPushConstants),
        &vpc
    );

    VkBufferMemoryBarrier voxelBufferBarrier{};
    voxelBufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    voxelBufferBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
    voxelBufferBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    voxelBufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    voxelBufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    voxelBufferBarrier.buffer = m_pPipeline->m_VoxelGPUBuffer;
    voxelBufferBarrier.offset = 0;
    voxelBufferBarrier.size = VK_WHOLE_SIZE;
    vkCmdPipelineBarrier(
        m_CommandBuffer,
        VK_PIPELINE_STAGE_HOST_BIT,
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        0,
        0, nullptr,
        1, &voxelBufferBarrier,
        0, nullptr
    );

    uint32_t groupCountX = (m_pWindowDesc->Width + 15) / 16;
    uint32_t groupCountY = (m_pWindowDesc->Height + 15) / 16;
    vkCmdDispatch(m_CommandBuffer, groupCountX, groupCountY, 1);

    VkImageMemoryBarrier presentBarrier{};
    presentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    presentBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
    presentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    presentBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
    presentBarrier.dstAccessMask = 0;
    presentBarrier.image = m_pSwapChain->GetImage(imageIndex);
    presentBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    vkCmdPipelineBarrier(
        m_CommandBuffer,
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &presentBarrier
    );

    vkEndCommandBuffer(m_CommandBuffer);

    VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffer;
    vkQueueSubmit(m_pDeviceAdapter->GetQueueHandle(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_pDeviceAdapter->GetQueueHandle());

    VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
    VkSwapchainKHR swapChain = m_pSwapChain->GetSwapChainHandle();
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapChain;
    presentInfo.pImageIndices = &imageIndex;
    vkQueuePresentKHR(m_pDeviceAdapter->GetQueueHandle(), &presentInfo);
}

// ---------------------------------------------------------------------------------------------------------------------
void Renderer::Destroy()
{
    vkDeviceWaitIdle(m_pDeviceAdapter->GetAdapterHandle());

    if (m_CommandBuffer != VK_NULL_HANDLE) {
        vkFreeCommandBuffers(m_pDeviceAdapter->GetAdapterHandle(), m_CommandPool, 1, &m_CommandBuffer);
        m_CommandBuffer = VK_NULL_HANDLE;
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
VkCommandPool Renderer::CreateCommandPool(shared_ptr<const DeviceAdapter> da, uint32_t uQueueFamily)
{
    VkCommandPool           cmdPool;
    VkCommandPoolCreateInfo cmdPoolInfo;

    cmdPoolInfo.sType               = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.pNext               = NULL;
    cmdPoolInfo.flags               = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    cmdPoolInfo.queueFamilyIndex    = uQueueFamily;

    ThrowIfFailed(vkCreateCommandPool(da->GetAdapterHandle(), &cmdPoolInfo, NULL, &cmdPool));

    return cmdPool;
}
 
// ---------------------------------------------------------------------------------------------------------------------
VkCommandBuffer Renderer::CreateCommandBuffer(::std::shared_ptr<const DeviceAdapter> da, VkCommandPool cmdPool)
{
    VkCommandBuffer             cmdBuffer;
    VkCommandBufferAllocateInfo allocInfo;
    VkDevice device = m_pDeviceAdapter->GetAdapterHandle();
    VkFence inFlightFence;

    allocInfo.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext                 = NULL;
    allocInfo.commandPool           = cmdPool;
    allocInfo.level                 = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount    = 1;

    ThrowIfFailed(vkAllocateCommandBuffers(da->GetAdapterHandle(), &allocInfo, &cmdBuffer));
    ThrowIfFailed(vkResetCommandBuffer(cmdBuffer, 0));

    return cmdBuffer;
}

} // !Voxels
