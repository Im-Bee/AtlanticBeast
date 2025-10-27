#ifndef AB_WRAPPER_ADAPTER_H
#define AB_WRAPPER_ADAPTER_H

namespace Voxels
{

class Adapter
{
public:

    Adapter() = delete;

    Adapter(VkPhysicalDevice gpu, 
            const uint32_t uFlags,
            const std::vector<const char*>& vExtensions,
            const void* pFeatures)
        : m_uQueueFamily(ChooseQueueFamily(gpu, uFlags))
        , m_Device(CreateDevice(gpu, vExtensions, pFeatures, m_uQueueFamily))
        , m_Queue(CreateQueue(m_Device, m_uQueueFamily))
    { }

    Adapter(Adapter&&) noexcept = default;
    Adapter(const Adapter&) = delete;
                   
    ~Adapter()
    {
        if (m_Device != VK_NULL_HANDLE) {
            vkDestroyDevice(m_Device, NULL);
            m_Device = VK_NULL_HANDLE;
        }
    }

public:

    uint32_t GetQueueFamilyIndex() const
    { return m_uQueueFamily; }

    VkDevice GetAdapterHandle() const
    { return m_Device; }

    VkQueue GetQueueHandle() const
    { return m_Queue; }

private:

    uint32_t ChooseQueueFamily(VkPhysicalDevice gpu, const uint32_t uFlags) const;

    VkDevice CreateDevice(VkPhysicalDevice gpu, 
                          const std::vector<const char*>& vExtensions,
                          const void* pFeatures,
                          const uint32_t uFamilyIndex) const;
    
    VkQueue CreateQueue(VkDevice dv, uint32_t uQueueIndex) const;

private:

    uint32_t m_uQueueFamily = 0;

    VkDevice    m_Device    = VK_NULL_HANDLE;
    VkQueue     m_Queue     = VK_NULL_HANDLE;

};

} // !Voxels
#endif // !AB_WRAPPER_ADAPTER_H

