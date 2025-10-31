#ifndef AB_MEMORY_H
#define AB_MEMORY_H

#include "Voxels.hpp"

#include "WrapperHardware.hpp"
#include "WrapperAdapter.hpp"
#include "GPUBuffer.hpp"
#include "GPUStreamBuffer.hpp"

namespace Voxels
{


struct UploadDescriptor
{
    UploadDescriptor() = delete;

    UploadDescriptor(VkDescriptorBufferInfo bufferInfo,
                     VkWriteDescriptorSet write)
        : BufferInfo(bufferInfo)
        , Write(write)
    {
        Write.pBufferInfo = &BufferInfo;
    }

    VkDescriptorBufferInfo BufferInfo;
    VkWriteDescriptorSet Write;
};


class Memory 
{
public:
    
    Memory() = default;

    Memory(::std::shared_ptr<const HardwareWrapper> pHardware,
           ::std::shared_ptr<const AdapterWrapper> pAdapter);

    ~Memory() = default;

public:

    Memory(const Memory&) noexcept = default;
    Memory& operator=(const Memory&) noexcept = default;

    Memory(Memory&&) noexcept = default;
    Memory& operator=(Memory&&) noexcept = default;

public:

    BEAST_API GPUStreamBuffer ReserveStagingBuffer(const size_t uSizeInBytes);

    BEAST_API GPUBuffer ReserveGPUBuffer(const size_t uSizeInBytes);

    BEAST_API void UploadOnStreamBuffer(const void* pUpload, 
                                        GPUStreamBuffer& outBuffer,
                                        const UploadDescriptor& onSet);
private:

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

private:

    ::std::shared_ptr<const HardwareWrapper> m_pHardware = nullptr;
    ::std::shared_ptr<const AdapterWrapper> m_pAdapter = nullptr;

};

} //!Voxels

#endif //!AB_MEMORY_H
