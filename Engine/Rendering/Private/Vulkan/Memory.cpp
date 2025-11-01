#include "Vulkan/Memory.hpp"

#include "Core.h"
#include "Debug/Assert.hpp"
#include "Vulkan/ErrorHandling.hpp"
#include <memory>

namespace Voxels
{

using namespace std;

// --------------------------------------------------------------------------------------------------------------------
Memory::Memory(shared_ptr<const HardwareWrapper> pHardware, 
               shared_ptr<const AdapterWrapper> pAdapter)
    : m_pHardware(pHardware)
    , m_pAdapter(pAdapter)
{ }

// --------------------------------------------------------------------------------------------------------------------
shared_ptr<GPUStreamBuffer> Memory::ReserveStagingBuffer(const size_t uSizeInBytes)
{
    AB_LOG(Core::Debug::Info, L"Reserving staging buffer of %llu bytes", uSizeInBytes);

    const VkDevice da = m_pAdapter->GetAdapterHandle();
    VkMemoryRequirements    memRequirements;
    VkBuffer                voxelBuffer;
    VkDeviceMemory          voxelBufferMemory;

    VkBufferCreateInfo bufferInfo = { };
    bufferInfo.sType        = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size         = uSizeInBytes;
    bufferInfo.usage        = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferInfo.sharingMode  = VK_SHARING_MODE_EXCLUSIVE;

    THROW_IF_FAILED(vkCreateBuffer(da, 
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

    return make_shared<GPUStreamBuffer>(m_pAdapter, voxelBufferMemory, voxelBuffer, nullptr, uSizeInBytes);
}

// --------------------------------------------------------------------------------------------------------------------
shared_ptr<GPUBuffer> Memory::ReserveGPUBuffer(const size_t uSizeInBytes)
{
    AB_LOG(Core::Debug::Info, L"Reserving gpu buffer of %llu bytes", uSizeInBytes);

    const VkDevice da = m_pAdapter->GetAdapterHandle();
    VkMemoryRequirements    memRequirements;
    VkBuffer                buffer;
    VkDeviceMemory          deviceMem;

    VkBufferCreateInfo bufferInfo = { };
    bufferInfo.sType        = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size         = uSizeInBytes;
    bufferInfo.usage        = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferInfo.sharingMode  = VK_SHARING_MODE_EXCLUSIVE;

    THROW_IF_FAILED(vkCreateBuffer(da, 
                                   &bufferInfo,
                                   NULL,
                                   &buffer));

    vkGetBufferMemoryRequirements(da, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = { };
    allocInfo.sType             = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize    = memRequirements.size;
    allocInfo.memoryTypeIndex   = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    THROW_IF_FAILED(vkAllocateMemory(da, &allocInfo, NULL, &deviceMem));
    THROW_IF_FAILED(vkBindBufferMemory(da, buffer, deviceMem, 0));

    return make_shared<GPUBuffer>(m_pAdapter, deviceMem, buffer, uSizeInBytes);
}

// --------------------------------------------------------------------------------------------------------------------
void Memory::UploadOnStreamBuffer(const void* pUpload, 
                                  UploadDescriptor& onSet)
{
    if (onSet.Type != UploadDescriptor::EUploadType::StreamBuffer) {
        throw AB_EXCEPT("UploadOnStreamBuffer, type of buffer is invalid");
    }
    if (onSet.LocalBuf.expired()) {
        throw AB_EXCEPT("UploadOnStreamBuffer, buffer is expired");
    }
    
	auto lock = onSet.LocalBuf.lock();
    GPUStreamBuffer* buf = reinterpret_cast<GPUStreamBuffer*>(lock.get());
    AB_ASSERT((buf->GetMemoryHandle() != VK_NULL_HANDLE));
    AB_ASSERT((buf->GetBufferHandle() != VK_NULL_HANDLE));

    const VkDevice da = m_pAdapter->GetAdapterHandle();
    bool updateDescSets = buf->GetDataPointer() == nullptr ? true : false;
    
    if (buf->GetDataPointer() == nullptr) {
        THROW_IF_FAILED(vkMapMemory(da, 
                                    buf->GetMemoryHandle(),
                                    0,
                                    buf->GetSizeInBytes(),
                                    0,
                                    buf->GetPtrToDataPointer()));
    }
    memcpy(buf->GetDataPointer(), pUpload, buf->GetSizeInBytes());

    if (updateDescSets) {
        vkUpdateDescriptorSets(da,
                               1,
                               &onSet.Write,
                               0,
                               NULL);
    }
}

// ---------------------------------------------------------------------------------------------------------------------
uint32_t Memory::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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

} //!Voxels
