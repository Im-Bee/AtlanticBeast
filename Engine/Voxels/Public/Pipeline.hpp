#ifndef AB_PIPELINE_H
#define AB_PIPELINE_H

#include "Voxels.hpp"
#include "DeviceAdapter.hpp"
#include "PushConstants.hpp"

namespace Voxels
{

class BEAST_VOXEL_API Pipeline
{

public:

    Pipeline(::std::shared_ptr<const DeviceAdapter> da);

    ~Pipeline() = default;

private:

    VkDescriptorSetLayout CreateDescriptorLayout(::std::shared_ptr<const DeviceAdapter>& da);

    VkPipelineLayout CreatePipelineLayout(::std::shared_ptr<const DeviceAdapter>& da,
                                          VkDescriptorSetLayout descriptorSetLayout);

    VkShaderModule LoadShader(::std::shared_ptr<const DeviceAdapter>& da, const ::std::string& strPath);

    VkPipeline CreateComputePipeline(::std::shared_ptr<const DeviceAdapter>& da, VkPipelineLayout pipelineLayout);

private:

    ::std::shared_ptr<const DeviceAdapter> m_pDeviceAdapter = nullptr;

    VkDescriptorSetLayout   m_DescriptorLayout  = VK_NULL_HANDLE;
    VkPipelineLayout        m_PipelineLayout    = VK_NULL_HANDLE;
    VkPipeline              m_ComputePipeline   = VK_NULL_HANDLE;

};

} // !Voxels

#endif // !AB_PIPELINE_H

