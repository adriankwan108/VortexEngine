#include "VulkanBuffer.hpp"

namespace vkclass
{
    vkclass::VulkanDevice* VulkanBuffer::m_device = nullptr;

    VulkanBuffer::VulkanBuffer(VkDeviceSize size, VkBufferUsageFlags flags):
        m_size(size)
    {
        // buffer
        VkBufferCreateInfo bufferCI{};
        bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCI.size = m_size;
        bufferCI.usage = flags;
        bufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        VK_CHECK_RESULT(vkCreateBuffer(m_device->LogicalDevice, &bufferCI, nullptr, &m_buffer));

        // memory
        VkDeviceSize allocationSize; // depends on OS
        uint32_t memoryType;
        m_device->GetMemoryInfo(m_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, allocationSize, memoryType);
        
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = allocationSize;
        allocInfo.memoryTypeIndex = memoryType;
        
        VK_CHECK_RESULT(vkAllocateMemory(m_device->LogicalDevice, &allocInfo, nullptr, &m_memory));
    }

    VulkanBuffer::VulkanBuffer(void* data, VkDeviceSize size, VkBufferUsageFlags flags):
        m_size(size)
    {
        // buffer
        VkBufferCreateInfo bufferCI{};
        bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCI.size = m_size;
        bufferCI.usage = flags;
        bufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        VK_CHECK_RESULT(vkCreateBuffer(m_device->LogicalDevice, &bufferCI, nullptr, &m_buffer));

        // memory
        VkDeviceSize allocationSize; // depends on OS
        uint32_t memoryType;
        m_device->GetMemoryInfo(m_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, allocationSize, memoryType);
        
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = allocationSize;
        allocInfo.memoryTypeIndex = memoryType;
        
        VK_CHECK_RESULT(vkAllocateMemory(m_device->LogicalDevice, &allocInfo, nullptr, &m_memory));
        
        Bind();
        Map();
        SetData(data, size);
        Unmap();
    }

    VulkanBuffer::~VulkanBuffer()
    {
        if(m_buffer)
        {
            vkDestroyBuffer(m_device->LogicalDevice, m_buffer, nullptr);
        }
        
        if(m_memory)
        {
            vkFreeMemory(m_device->LogicalDevice, m_memory, nullptr);
        }
    }

    void VulkanBuffer::Init(vkclass::VulkanDevice *device)
    {
        m_device = device;
    }

    void VulkanBuffer::Bind(VkDeviceSize offset)
    {
        // TODO: if offset is non-zero, then it must be divisible by memRequirements.alignment
        vkBindBufferMemory(m_device->LogicalDevice, m_buffer, m_memory, offset);
    }

    void VulkanBuffer::Map()
    {
        vkMapMemory(m_device->LogicalDevice, m_memory, 0, m_size, 0, &m_data);
    }

    void VulkanBuffer::Unmap()
    {
        if(m_memory)
        {
            vkUnmapMemory(m_device->LogicalDevice, m_memory);
        }
    }

    void VulkanBuffer::SetData(void* data, VkDeviceSize size)
    {
        memcpy(m_data, data, size);
    }

    void VulkanBuffer::Flush(VkDeviceSize size, VkDeviceSize offset) const
    {
        // flush driver's cache so that enables immediate copy data to buffer memory
        // this is not needed if VK_MEMORY_PROPERTY_HOST_COHERENT_BIT property is set
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = m_memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        vkFlushMappedMemoryRanges(m_device->LogicalDevice, 1, &mappedRange);
    }
}
