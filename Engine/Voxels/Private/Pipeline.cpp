#include "Pipeline.hpp"

#include "ErrorHandling.hpp"

namespace Voxels
{

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
Pipeline::Pipeline(::std::shared_ptr<const DeviceAdapter> da)
    : m_pDeviceAdapter(da)
    , m_DescriptorLayout(CreateDescriptorLayout(m_pDeviceAdapter))
    , m_PipelineLayout(CreatePipelineLayout(m_pDeviceAdapter, m_DescriptorLayout))
    , m_ComputePipeline(CreateComputePipeline(m_pDeviceAdapter, m_PipelineLayout))
{ }

// ---------------------------------------------------------------------------------------------------------------------
VkDescriptorSetLayout Pipeline::CreateDescriptorLayout(shared_ptr<const DeviceAdapter>& da)
{
    array<VkDescriptorSetLayoutBinding, 2>  bindings                = { };
    VkDescriptorSetLayoutCreateInfo         layoutCreateInfo;
    VkDescriptorSetLayout                   descriptorSetLayout;

    bindings[0].binding             = 0;
    bindings[0].descriptorType      = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    bindings[0].descriptorCount     = 1;
    bindings[0].stageFlags          = VK_SHADER_STAGE_COMPUTE_BIT;
    bindings[0].pImmutableSamplers  = NULL;

    bindings[1].binding             = 1;
    bindings[1].descriptorType      = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[1].descriptorCount     = 1;
    bindings[1].stageFlags          = VK_SHADER_STAGE_COMPUTE_BIT;
    bindings[1].pImmutableSamplers  = NULL;

    layoutCreateInfo.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.pNext          = NULL;
    layoutCreateInfo.flags          = 0;
    layoutCreateInfo.bindingCount   = static_cast<uint32_t>(bindings.size());
    layoutCreateInfo.pBindings      = &bindings[0];

    ThrowIfFailed(vkCreateDescriptorSetLayout(da->GetAdapterHandle(), 
                                              &layoutCreateInfo,
                                              nullptr,
                                              &descriptorSetLayout));

    return descriptorSetLayout;
}

// ---------------------------------------------------------------------------------------------------------------------
VkPipelineLayout Pipeline::CreatePipelineLayout(shared_ptr<const DeviceAdapter>& da,
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

    ThrowIfFailed(vkCreatePipelineLayout(da->GetAdapterHandle(), 
                                         &layoutInfo,
                                         NULL,
                                         &pipelineLayout));

    return pipelineLayout;
}

// ---------------------------------------------------------------------------------------------------------------------
VkShaderModule Pipeline::LoadShader(shared_ptr<const DeviceAdapter>& da, const string& strPath)
{
    vector<char>                vBuffer;
    size_t                      uFileSize;
    VkShaderModuleCreateInfo    shaderCreateInfo;
    VkShaderModule              shaderModule;

    ifstream file(strPath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw AB_EXCEPT("Failed to open shader file!");
    }

    uFileSize = static_cast<size_t>(file.tellg());
    vBuffer.resize(uFileSize);
    
    file.seekg(0);
    file.read(&vBuffer[0], uFileSize);

    file.close();
    
    shaderCreateInfo.sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderCreateInfo.pNext      = NULL;
    shaderCreateInfo.flags      = 0;
    shaderCreateInfo.codeSize   = vBuffer.size(); // Size is in bytes, so it's okay
    shaderCreateInfo.pCode      = reinterpret_cast<const uint32_t*>(&vBuffer[0]);

    ThrowIfFailed(vkCreateShaderModule(da->GetAdapterHandle(), &shaderCreateInfo, nullptr, &shaderModule));

    return shaderModule;
}

// ---------------------------------------------------------------------------------------------------------------------
VkPipeline Pipeline::CreateComputePipeline(shared_ptr<const DeviceAdapter>& da, VkPipelineLayout pipelineLayout)
{ 
    VkPipeline                      pipeline        = VK_NULL_HANDLE;
    VkDevice                        device          = da->GetAdapterHandle();
    VkComputePipelineCreateInfo     pipelineInfo;
    VkPipelineShaderStageCreateInfo shaderStage;


    shaderStage.sType                   = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage.pNext                   = NULL;
    shaderStage.flags                   = 0;
    shaderStage.pSpecializationInfo     = NULL;
    shaderStage.stage                   = VK_SHADER_STAGE_COMPUTE_BIT;
    shaderStage.module                  = LoadShader(da, "./Assets/Raycast.comp.spv");
    shaderStage.pName                   = "main";


    pipelineInfo.sType              = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext              = NULL;
    pipelineInfo.flags              = 0;
    pipelineInfo.stage              = shaderStage;
    pipelineInfo.layout             = pipelineLayout;
    pipelineInfo.basePipelineHandle = NULL;
    pipelineInfo.basePipelineIndex  = 0;


    ThrowIfFailed(vkCreateComputePipelines(device,
                                           VK_NULL_HANDLE,
                                           1,
                                           &pipelineInfo,
                                           NULL,
                                           &pipeline));

    return pipeline;
}

}

