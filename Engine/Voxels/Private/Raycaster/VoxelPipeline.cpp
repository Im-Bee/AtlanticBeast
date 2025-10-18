#include "Raycaster/VoxelPipeline.hpp"

#include "Core.h"
#include "Debug/Logger.hpp"
#include "Vulkan/ErrorHandling.hpp"
#include "Vulkan/GPUStreamBuffer.hpp"
#include "Vulkan/SwapChain.hpp"

namespace Voxels
{

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
VoxelPipeline::VoxelPipeline(::std::shared_ptr<const Hardware> hw, 
                   ::std::shared_ptr<const Adapter> da)
    : m_pHardware(hw)
    , m_pDeviceAdapter(da)
    , m_VoxelGrid(nullptr)
    , m_DescriptorLayout(CreateDescriptorLayout(m_pDeviceAdapter))
    , m_DescriptorPool(CreateDescriptorPool(m_pDeviceAdapter))
    , m_DescriptorSet(CreateDescriptorSet(m_pDeviceAdapter, m_DescriptorPool, m_DescriptorLayout))
    , m_PipelineLayout(CreatePipelineLayout(m_pDeviceAdapter, m_DescriptorLayout))
    , m_ShaderModule(LoadShader(m_pDeviceAdapter, 
                                App::AppResources::Get().GetExecutablePathA() + "/Assets/Shaders/Raycast.comp.spv"))
    , m_ComputePipeline(CreateComputePipeline(m_pDeviceAdapter, m_PipelineLayout, m_ShaderModule))
{
    AB_LOG(Core::Debug::Info, L"Creating a pipeline!");
}

// ---------------------------------------------------------------------------------------------------------------------
VoxelPipeline::~VoxelPipeline()
{
    if (m_ImageView != VK_NULL_HANDLE) {
        vkDestroyImageView(m_pDeviceAdapter->GetAdapterHandle(), m_ImageView, nullptr);
        m_ImageView = VK_NULL_HANDLE;
    }
    if (m_ComputePipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(m_pDeviceAdapter->GetAdapterHandle(), m_ComputePipeline, NULL);
        m_ComputePipeline = VK_NULL_HANDLE;
    }
    if (m_ShaderModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(m_pDeviceAdapter->GetAdapterHandle(), m_ShaderModule, NULL);
        m_ShaderModule = VK_NULL_HANDLE;
    }
    if (m_PipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(m_pDeviceAdapter->GetAdapterHandle(), m_PipelineLayout, NULL);
        m_PipelineLayout = VK_NULL_HANDLE;
    }
    if (m_DescriptorPool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(m_pDeviceAdapter->GetAdapterHandle(), m_DescriptorPool, NULL);
        m_DescriptorPool = VK_NULL_HANDLE;
    }
    if (m_DescriptorLayout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(m_pDeviceAdapter->GetAdapterHandle(), m_DescriptorLayout, NULL);
        m_DescriptorLayout = VK_NULL_HANDLE;
    }
}

// Public // -----------------------------------------------------------------------------------------------------------
GPUStreamBuffer VoxelPipeline::ReserveGridBuffer(const shared_ptr<const VoxelGrid>& vg)
{
    const VkDeviceSize      uBufferSizeInBytes      = vg->GetAmountOfVoxels() * sizeof(Voxel);
    const VkDevice          da                      = m_pDeviceAdapter->GetAdapterHandle();
    VkMemoryRequirements    memRequirements;
    VkBuffer                voxelBuffer;
    VkDeviceMemory          voxelBufferMemory;

    VkBufferCreateInfo bufferInfo = { };
    bufferInfo.sType        = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size         = uBufferSizeInBytes;
    bufferInfo.usage        = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    bufferInfo.sharingMode  = VK_SHARING_MODE_EXCLUSIVE;

    THROW_IF_FAILED(vkCreateBuffer(m_pDeviceAdapter->GetAdapterHandle(), 
                                   &bufferInfo,
                                   NULL,
                                   &voxelBuffer));

    vkGetBufferMemoryRequirements(da, voxelBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = { };
    allocInfo.sType             = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize    = memRequirements.size;
    allocInfo.memoryTypeIndex   = FindMemoryType(memRequirements.memoryTypeBits, 
                                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
                                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    THROW_IF_FAILED(vkAllocateMemory(da, &allocInfo, NULL, &voxelBufferMemory));
    THROW_IF_FAILED(vkBindBufferMemory(da, voxelBuffer, voxelBufferMemory, 0));

    m_VoxelGrid = vg;
    int32_t w = static_cast<int32_t>(m_VoxelGrid->GetGridWidth());
    m_Vpc.GridSize = iVec4(w, w, w);

    return GPUStreamBuffer(m_pDeviceAdapter, voxelBufferMemory, voxelBuffer, nullptr, uBufferSizeInBytes);
}

// ---------------------------------------------------------------------------------------------------------------------
void VoxelPipeline::LoadGrid(const shared_ptr<const VoxelGrid>& vg, GPUStreamBuffer& outBuffer)
{
    AB_ASSERT((outBuffer.GetMemoryHandle() != VK_NULL_HANDLE));
    AB_ASSERT((outBuffer.GetBufferHandle() != VK_NULL_HANDLE));
    AB_ASSERT((outBuffer.GetSizeInBytes() == vg->GetAmountOfVoxels() * sizeof(Voxel)));
    AB_ASSERT((m_VoxelGrid != nullptr));
    AB_ASSERT((vg->GetAmountOfVoxels() == m_VoxelGrid->GetAmountOfVoxels()));

    const VkDevice da = m_pDeviceAdapter->GetAdapterHandle();
    
    if (*outBuffer.GetDataPointer() == nullptr) {
        THROW_IF_FAILED(vkMapMemory(da, 
                                    outBuffer.GetMemoryHandle(),
                                    0,
                                    outBuffer.GetSizeInBytes(),
                                    0,
                                    outBuffer.GetDataPointer()));
    }
    memcpy(*outBuffer.GetDataPointer(), &vg->GetGrid()[0], outBuffer.GetSizeInBytes());

    VkDescriptorBufferInfo voxelBufferInfo = { };
    voxelBufferInfo.buffer  = outBuffer.GetBufferHandle();
    voxelBufferInfo.offset  = 0;
    voxelBufferInfo.range   = VK_WHOLE_SIZE;

    VkWriteDescriptorSet voxelWrite = { };
    voxelWrite.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    voxelWrite.dstSet           = m_DescriptorSet;
    voxelWrite.dstBinding       = 1;
    voxelWrite.descriptorType   = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    voxelWrite.descriptorCount  = 1;
    voxelWrite.pBufferInfo      = &voxelBufferInfo;

    vkUpdateDescriptorSets(da,
                           1,
                           &voxelWrite,
                           0,
                           NULL);
}

// ---------------------------------------------------------------------------------------------------------------------
void VoxelPipeline::LoadImage(VkImage image)
{
    if (m_ImageView != VK_NULL_HANDLE) {
        vkDestroyImageView(m_pDeviceAdapter->GetAdapterHandle(), m_ImageView, nullptr);
        m_ImageView = VK_NULL_HANDLE;
    }

    VkImageViewCreateInfo viewInfo = { };
    viewInfo.sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image      = image;
    viewInfo.viewType   = VK_IMAGE_VIEW_TYPE_2D; 
    viewInfo.format     = Swapchain::TargetedFormat;
    viewInfo.subresourceRange.aspectMask        = VK_IMAGE_ASPECT_COLOR_BIT; 
    viewInfo.subresourceRange.baseMipLevel      = 0;
    viewInfo.subresourceRange.levelCount        = 1;
    viewInfo.subresourceRange.baseArrayLayer    = 0;
    viewInfo.subresourceRange.layerCount        = 1;

    THROW_IF_FAILED(vkCreateImageView(m_pDeviceAdapter->GetAdapterHandle(), 
                                      &viewInfo,
                                      nullptr,
                                      &m_ImageView));

    VkDescriptorImageInfo imageInfo = { };
    imageInfo.imageView     = m_ImageView;
    imageInfo.imageLayout   = VK_IMAGE_LAYOUT_GENERAL; 

    VkWriteDescriptorSet imageWrite = { };
    imageWrite.sType    = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    imageWrite.dstSet   = m_DescriptorSet;
    imageWrite.dstBinding       = 0; 
    imageWrite.dstArrayElement  = 0;
    imageWrite.descriptorType   = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    imageWrite.descriptorCount  = 1;
    imageWrite.pImageInfo       = &imageInfo;
    
    vkUpdateDescriptorSets(m_pDeviceAdapter->GetAdapterHandle(), 
                           1,
                           &imageWrite,
                           0,
                           nullptr);
}


// Private // ----------------------------------------------------------------------------------------------------------
VkDescriptorSetLayout VoxelPipeline::CreateDescriptorLayout(shared_ptr<const Adapter>& da)
{
    array<VkDescriptorSetLayoutBinding, 2>  bindings    = { };
    VkDescriptorSetLayout                   descriptorSetLayout;

    bindings[0].binding             = 0;
    bindings[0].descriptorType      = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    bindings[0].descriptorCount     = 1;
    bindings[0].stageFlags          = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[1].binding             = 1;
    bindings[1].descriptorType      = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[1].descriptorCount     = 1;
    bindings[1].stageFlags          = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo = { };
    layoutCreateInfo.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount   = static_cast<uint32_t>(bindings.size());
    layoutCreateInfo.pBindings      = &bindings[0];

    THROW_IF_FAILED(vkCreateDescriptorSetLayout(da->GetAdapterHandle(), 
                                                &layoutCreateInfo,
                                                NULL,
                                                &descriptorSetLayout));

    return descriptorSetLayout;
}

// ---------------------------------------------------------------------------------------------------------------------
VkDescriptorPool VoxelPipeline::CreateDescriptorPool(shared_ptr<const Adapter>& da)
{
    const VkDescriptorPoolSize  poolSizes[] = {
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,  1 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1 },
    };

    VkDescriptorPool descriptorPool;

    VkDescriptorPoolCreateInfo poolInfo = { };
    poolInfo.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount  = 2;
    poolInfo.pPoolSizes     = poolSizes;
    poolInfo.maxSets        = 1;

    THROW_IF_FAILED(vkCreateDescriptorPool(da->GetAdapterHandle(), &poolInfo, NULL, &descriptorPool));

    return descriptorPool;
}

// ---------------------------------------------------------------------------------------------------------------------
VkDescriptorSet VoxelPipeline::CreateDescriptorSet(shared_ptr<const Adapter>& da,
                                                   VkDescriptorPool descPool,
                                                   VkDescriptorSetLayout descLayout)
{
    VkDescriptorSet descriptorSet;

    VkDescriptorSetAllocateInfo allocInfo = { };
    allocInfo.sType                 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool        = descPool;
    allocInfo.descriptorSetCount    = 1;
    allocInfo.pSetLayouts           = &descLayout;

    THROW_IF_FAILED(vkAllocateDescriptorSets(da->GetAdapterHandle(), &allocInfo, &descriptorSet));

    return descriptorSet;
}

// ---------------------------------------------------------------------------------------------------------------------
VkPipelineLayout VoxelPipeline::CreatePipelineLayout(shared_ptr<const Adapter>& da,
                                                     VkDescriptorSetLayout descriptorSetLayout)
{ 
    VkPushConstantRange         pushConstantRange;
    VkPipelineLayoutCreateInfo  layoutInfo;
    VkPipelineLayout            pipelineLayout;

    pushConstantRange.stageFlags    = VK_SHADER_STAGE_COMPUTE_BIT;
    pushConstantRange.offset        = 0;
    pushConstantRange.size          = sizeof(VoxelPushConstants);

    layoutInfo.sType                    = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.pNext                    = NULL;
    layoutInfo.flags                    = 0;
    layoutInfo.setLayoutCount           = 1;
    layoutInfo.pSetLayouts              = &descriptorSetLayout;
    layoutInfo.pushConstantRangeCount   = 1;
    layoutInfo.pPushConstantRanges      = &pushConstantRange;

    THROW_IF_FAILED(vkCreatePipelineLayout(da->GetAdapterHandle(), 
                                         &layoutInfo,
                                         NULL,
                                         &pipelineLayout));

    return pipelineLayout;
}

// ---------------------------------------------------------------------------------------------------------------------
VkShaderModule VoxelPipeline::LoadShader(shared_ptr<const Adapter>& da, const string& strPath)
{
    vector<char>                vBuffer;
    size_t                      uFileSize;
    VkShaderModule              shaderModule;

    ifstream file(strPath, ios::ate | ios::binary);

    if (!file.is_open()) {
        AB_LOG(Core::Debug::Error, L"%s", strPath.c_str());
        throw AB_EXCEPT("Failed to open shader file!");
    }

    uFileSize = static_cast<size_t>(file.tellg());
    vBuffer.resize(uFileSize);
    
    file.seekg(0);
    file.read(&vBuffer[0], uFileSize);

    file.close();
    
    VkShaderModuleCreateInfo shaderCreateInfo = { };
    shaderCreateInfo.sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderCreateInfo.codeSize   = vBuffer.size(); // Size is in bytes, so it's okay
    shaderCreateInfo.pCode      = reinterpret_cast<const uint32_t*>(&vBuffer[0]);

    THROW_IF_FAILED(vkCreateShaderModule(da->GetAdapterHandle(), &shaderCreateInfo, NULL, &shaderModule));

    return shaderModule;
}

// ---------------------------------------------------------------------------------------------------------------------
VkPipeline VoxelPipeline::CreateComputePipeline(shared_ptr<const Adapter>& da, 
                                                VkPipelineLayout pipelineLayout, 
                                                VkShaderModule shaderModule)
{ 
    const VkDevice  device      = da->GetAdapterHandle();
    VkPipeline      pipeline    = VK_NULL_HANDLE;

    VkPipelineShaderStageCreateInfo shaderStage = { };
    shaderStage.sType                   = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage.stage                   = VK_SHADER_STAGE_COMPUTE_BIT;
    shaderStage.module                  = shaderModule;
    shaderStage.pName                   = "main";


    VkComputePipelineCreateInfo pipelineInfo = { };
    pipelineInfo.sType              = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage              = shaderStage;
    pipelineInfo.layout             = pipelineLayout;

    THROW_IF_FAILED(vkCreateComputePipelines(device,
                                           VK_NULL_HANDLE,
                                           1,
                                           &pipelineInfo,
                                           NULL,
                                           &pipeline));

    return pipeline;
}

// ---------------------------------------------------------------------------------------------------------------------
uint32_t VoxelPipeline::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;

    vkGetPhysicalDeviceMemoryProperties(m_pHardware->GetPhysicalDevice(), &memProperties); 

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) 
    {
        bool bTypeMatch          = (typeFilter & (1 << i)) != 0;
        bool bPropertiesMatch    = (memProperties.memoryTypes[i].propertyFlags & properties) == properties;

        if (bTypeMatch && bPropertiesMatch) {
            return i;
        }
    }

    throw AB_EXCEPT("Failed to find suitable memory type!");
}

} // !Voxels

