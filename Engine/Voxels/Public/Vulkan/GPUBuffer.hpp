#ifndef AB_GPU_BUFFER_H
#define AB_GPU_BUFFER_H

#include "Voxels.hpp"
#include "WrapperAdapter.hpp"

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

    GPUBuffer(::std::shared_ptr<const Adapter> da,
              VkDeviceMemory deviceMemory,
              VkBuffer buffer,
              size_t sizeInBytes) 
        : m_pDeviceAdapter(da)
        , m_DeviceMemory(deviceMemory)
        , m_Buffer(buffer)
        , m_uSizeInBytes(sizeInBytes)
    { }

    GPUBuffer(const GPUBuffer& other) = delete;

    GPUBuffer(GPUBuffer&& other) noexcept
        : m_pDeviceAdapter(std::move(other.m_pDeviceAdapter))
        , m_DeviceMemory(other.m_DeviceMemory)
        , m_Buffer(other.m_Buffer)
        , m_uSizeInBytes(other.m_uSizeInBytes)
    {
        other.m_DeviceMemory = VK_NULL_HANDLE;
        other.m_Buffer = VK_NULL_HANDLE;
    }

    ~GPUBuffer() 
    {
        if (m_pDeviceAdapter == nullptr) {
            return;
        }
        if (m_Buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(m_pDeviceAdapter->GetAdapterHandle(), m_Buffer, NULL);
        }
        if (m_DeviceMemory != VK_NULL_HANDLE) {
            vkFreeMemory(m_pDeviceAdapter->GetAdapterHandle(), m_DeviceMemory, NULL);
        }
    }

public:

    GPUBuffer& operator=(GPUBuffer&& other) noexcept
    {
        m_pDeviceAdapter = std::move(other.m_pDeviceAdapter);
        m_DeviceMemory = other.m_DeviceMemory;
        m_Buffer = other.m_Buffer;
        m_uSizeInBytes = other.m_uSizeInBytes;

        other.m_DeviceMemory = VK_NULL_HANDLE;
        other.m_Buffer = VK_NULL_HANDLE;
        other.m_uSizeInBytes = 0;

        return *this; 
    }

public:

    VkDeviceMemory GetMemoryHandle() const 
    { return m_DeviceMemory; }

    VkBuffer GetBufferHandle() const 
    { return m_Buffer; }

    size_t GetSizeInBytes() const 
    { return m_uSizeInBytes; }

protected:

    ::std::shared_ptr<const Adapter> m_pDeviceAdapter = nullptr;
    VkDeviceMemory  m_DeviceMemory  = VK_NULL_HANDLE;
    VkBuffer        m_Buffer        = VK_NULL_HANDLE;
    size_t          m_uSizeInBytes  = 0;

};

} // !Voxels
#endif //!AB_GPU_BUFFER_H
