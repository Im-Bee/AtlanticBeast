#ifndef AB_WRAPPER_ADAPTER_H
#define AB_WRAPPER_ADAPTER_H

namespace Voxels
{

class WrapperAdapter
{
public:

    WrapperAdapter() = delete;

    WrapperAdapter(uint32_t uQueueIndex,
                   VkDevice device,
                   VkQueue queue)
        : m_uQueueFamily(uQueueIndex)
        , m_Device(device)
        , m_Queue(queue)
    { }

    ~WrapperAdapter()
    {
        if (m_Device != VK_NULL_HANDLE) {
            vkDestroyDevice(m_Device, nullptr);
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

    uint32_t m_uQueueFamily = 0;

    VkDevice    m_Device    = VK_NULL_HANDLE;
    VkQueue     m_Queue     = VK_NULL_HANDLE;

};

} // !Voxels
#endif // !AB_WRAPPER_ADAPTER_H

