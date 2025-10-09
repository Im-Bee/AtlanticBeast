#ifndef AB_WRAPPER_ADAPTER_H
#define AB_WRAPPER_ADAPTER_H

#include "Voxels.hpp"

namespace Voxels
{

class WrapperAdapter
{
public:

    WrapperAdapter()
        : m_uQueueFamily(0)
        , m_Device(VK_NULL_HANDLE)
        , m_Queue(VK_NULL_HANDLE)
    { }

    WrapperAdapter(uint32_t uQueueIndex,
                   VkDevice device,
                   VkQueue queue)
        : m_uQueueFamily(uQueueIndex)
        , m_Device(device)
        , m_Queue(queue)
    { }

    WrapperAdapter(VkPhysicalDevice gpu, 
                   uint32_t uFlags,
                   const std::vector<const char*>& vExtensions,
                   const void* pFeatures)
        : m_uQueueFamily(ChooseQueueFamily(gpu, uFlags))
        , m_Device(CreateDevice(gpu, vExtensions, pFeatures, m_uQueueFamily))
        , m_Queue(CreateQueue(m_Device, m_uQueueFamily))
    { }
                   

    ~WrapperAdapter()
    {
        if (m_Device != VK_NULL_HANDLE) {
            vkDestroyDevice(m_Device, nullptr);
            m_Device = VK_NULL_HANDLE;
        }
    }

public:

    BEAST_VOXEL_API void RecreateAdapter(uint32_t uQueueIndex,
                                         VkDevice device,
                                         VkQueue queue);

public:

    uint32_t GetQueueFamilyIndex() const
    { return m_uQueueFamily; }

    VkDevice GetAdapterHandle() const
    { return m_Device; }

    VkQueue GetQueueHandle() const
    { return m_Queue; }

private:

    uint32_t ChooseQueueFamily(VkPhysicalDevice gpu, uint32_t uFlags);

    VkDevice CreateDevice(VkPhysicalDevice gpu, 
                          const std::vector<const char*>& vExtensions,
                          const void* pFeatures,
                          uint32_t uFamilyIndex);
    
    VkQueue CreateQueue(VkDevice dv, uint32_t uQueueIndex);

private:

    uint32_t m_uQueueFamily = 0;

    VkDevice    m_Device    = VK_NULL_HANDLE;
    VkQueue     m_Queue     = VK_NULL_HANDLE;

};

} // !Voxels
#endif // !AB_WRAPPER_ADAPTER_H

