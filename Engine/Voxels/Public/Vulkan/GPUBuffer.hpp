#ifndef AB_GPU_BUFFER_H
#define AB_GPU_BUFFER_H

#include "Vulkan/RTXDeviceAdapter.hpp"

namespace Voxels 
{

class GPUBuffer 
{
public:

    GPUBuffer() 
        : m_pDeviceAdapter(nullptr)
        , m_DeviceMemory(VK_NULL_HANDLE)
        , m_Buffer(VK_NULL_HANDLE)
        , m_uSizeInBytes(0)
    { }

    GPUBuffer(::std::shared_ptr<const RTXDeviceAdapter> da,
              VkDeviceMemory deviceMemory,
              VkBuffer buffer,
              size_t sizeInBytes) 
        : m_pDeviceAdapter(da)
        , m_DeviceMemory(deviceMemory)
        , m_Buffer(buffer)
        , m_uSizeInBytes(sizeInBytes)
    { }

    ~GPUBuffer() 
    {
        if (m_Buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(m_pDeviceAdapter->GetAdapterHandle(), m_Buffer, NULL);
        }
        if (m_DeviceMemory != VK_NULL_HANDLE) {
            vkFreeMemory(m_pDeviceAdapter->GetAdapterHandle(), m_DeviceMemory, NULL);
        }
    }

public:

    GPUBuffer& operator=(GPUBuffer&& other) 
    {
        m_pDeviceAdapter = std::move(other.m_pDeviceAdapter);
        m_DeviceMemory = other.m_DeviceMemory;
        m_Buffer = other.m_Buffer;
        *const_cast<size_t*>(&m_uSizeInBytes) = other.m_uSizeInBytes;

        other.m_DeviceMemory = VK_NULL_HANDLE;
        other.m_Buffer = VK_NULL_HANDLE;

        return *this; 
    }

    VkDeviceMemory& operator&() 
    { return m_DeviceMemory; }

public:

    VkDeviceMemory GetMemoryHandle() const 
    { return m_DeviceMemory; }

    VkBuffer GetBufferHandle() const 
    { return m_Buffer; }

    size_t GetSizeInBytes() const 
    { return m_uSizeInBytes; }

private:

    ::std::shared_ptr<const RTXDeviceAdapter> m_pDeviceAdapter = nullptr;
    VkDeviceMemory  m_DeviceMemory  = VK_NULL_HANDLE;
    VkBuffer        m_Buffer        = VK_NULL_HANDLE;
    const size_t    m_uSizeInBytes  = 0;

};

} // !Voxels
#endif //!AB_GPU_BUFFER_H
