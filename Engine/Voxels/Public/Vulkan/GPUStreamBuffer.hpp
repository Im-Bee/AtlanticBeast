#ifndef AB_GPU_STREAM_BUFFER_H
#define AB_GPU_STREAM_BUFFER_H

#include "Voxels.hpp"
#include "Vulkan/GPUBuffer.hpp"

namespace Voxels 
{

class GPUStreamBuffer : public GPUBuffer 
{
public:

    GPUStreamBuffer() 
        : GPUBuffer() 
    { }

    GPUStreamBuffer(::std::shared_ptr<const Adapter> da,
                    VkDeviceMemory deviceMemory,
                    VkBuffer buffer,
                    void* pData,
                    size_t sizeInBytes) 
        : GPUBuffer(da, deviceMemory, buffer, sizeInBytes) 
    { }

    GPUStreamBuffer(const GPUStreamBuffer& other) = delete;

    GPUStreamBuffer(GPUStreamBuffer&& other) noexcept
        : GPUBuffer(std::move(other))
        , m_pData(other.m_pData)
    { }

    ~GPUStreamBuffer() 
    {
        if (m_pDeviceAdapter == nullptr) {
            return;
        }
        if (m_pData != nullptr) {
            vkUnmapMemory(m_pDeviceAdapter->GetAdapterHandle(), m_DeviceMemory);
            m_pData = nullptr;
        }

        GPUBuffer::~GPUBuffer();
    }

public:

    GPUStreamBuffer& operator=(GPUStreamBuffer&& other) noexcept
    {
        this->GPUBuffer::operator=(std::move(other));
        m_pData = other.m_pData;

        other.m_pData = nullptr;
        return *this;
    }

public:

    void** GetDataPointer()
    { return &m_pData; }

private:

    void* m_pData = nullptr;

};

} // !Voxels
#endif // !AB_GPU_STREAM_BUFFER_H
