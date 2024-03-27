#include "VulkanBuffer.hpp"

namespace vkclass
{
    vkclass::VulkanDevice* VulkanBuffer::m_device = nullptr;
    vkclass::VulkanCommandManager* VulkanBuffer::m_cmdManager = nullptr;

    static VkFormat ShaderDataTypeToVulkanFormat(VX::ShaderDataType type)
    {
        switch (type)
        {
            case VX::ShaderDataType::None:     return VK_FORMAT_UNDEFINED;
            case VX::ShaderDataType::Float:    return VK_FORMAT_R32_SFLOAT;
            case VX::ShaderDataType::Float2:   return VK_FORMAT_R32G32_SFLOAT;
            case VX::ShaderDataType::Float3:   return VK_FORMAT_R32G32B32_SFLOAT;
            case VX::ShaderDataType::Float4:   return VK_FORMAT_R32G32B32A32_SFLOAT;
            case VX::ShaderDataType::Mat3:     return VK_FORMAT_R32G32B32_SFLOAT;
            case VX::ShaderDataType::Mat4:     return VK_FORMAT_R32G32B32A32_SFLOAT;
            case VX::ShaderDataType::Int:      return VK_FORMAT_R32_SINT;
            case VX::ShaderDataType::Int2:     return VK_FORMAT_R32G32_SINT;
            case VX::ShaderDataType::Int3:     return VK_FORMAT_R32G32B32_SINT;
            case VX::ShaderDataType::Int4:     return VK_FORMAT_R32G32B32A32_SINT;
            case VX::ShaderDataType::Bool:     return VK_FORMAT_R8_UINT;
        }
    }

    static uint32_t ShaderDataTypeSize(VX::ShaderDataType type)
    {
        switch (type)
        {
            case VX::ShaderDataType::Float:    return 4;
            case VX::ShaderDataType::Float2:   return 4 * 2;
            case VX::ShaderDataType::Float3:   return 4 * 3;
            case VX::ShaderDataType::Float4:   return 4 * 4;
            case VX::ShaderDataType::Mat3:     return 4 * 3 * 3;
            case VX::ShaderDataType::Mat4:     return 4 * 4 * 4;
            case VX::ShaderDataType::Int:      return 4;
            case VX::ShaderDataType::Int2:     return 4 * 2;
            case VX::ShaderDataType::Int3:     return 4 * 3;
            case VX::ShaderDataType::Int4:     return 4 * 4;
            case VX::ShaderDataType::Bool:     return 1;
            default:                       VX_CORE_ASSERT(false, "Unknown Shader data type.");
        }
    }

    VulkanBuffer::VulkanBuffer(VkDeviceSize size, VkMemoryPropertyFlags props, VkBufferUsageFlags usage):
        m_size(size)
    {
        createBuffer(size, usage);
        createMemory(props);
        Bind();
    }

    VulkanBuffer::VulkanBuffer(void* data, VkDeviceSize size, VkMemoryPropertyFlags props, VkBufferUsageFlags usage):
        m_size(size)
    {
        createBuffer(size, usage);
        createMemory(props);
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

    void VulkanBuffer::Setup(VkDeviceSize size, VkMemoryPropertyFlags props, VkBufferUsageFlags usage)
    {
        createBuffer(size, usage);
        createMemory(props);
        Bind();
    }

    void VulkanBuffer::Init(vkclass::VulkanDevice *device, vkclass::VulkanCommandManager* cmdManager)
    {
        m_device = device;
        m_cmdManager = cmdManager;
    }

    void VulkanBuffer::CopyTo(VulkanBuffer* srcBuffer, VulkanBuffer* dstBuffer, VkDeviceSize size)
    {
        m_cmdManager->CopyBuffer(srcBuffer->Buffer, dstBuffer->Buffer, size);
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

    void VulkanBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage)
    {
        VkBufferCreateInfo bufferCI{};
        bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCI.size = size;
        bufferCI.usage = usage;
        bufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        VK_CHECK_RESULT(vkCreateBuffer(m_device->LogicalDevice, &bufferCI, nullptr, &m_buffer));
    }

    void VulkanBuffer::createMemory(VkMemoryPropertyFlags props)
    {
        VkDeviceSize allocationSize; // depends on OS
        uint32_t memoryType;
        m_device->GetMemoryInfo(m_buffer, props, allocationSize, memoryType);
        
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = allocationSize;
        allocInfo.memoryTypeIndex = memoryType;
        
        VK_CHECK_RESULT(vkAllocateMemory(m_device->LogicalDevice, &allocInfo, nullptr, &m_memory));
    }


    VulkanVertexBuffer::VulkanVertexBuffer(void* data, VkDeviceSize size)
    {
        // staging in CPU accesible memory
        VulkanBuffer stagingBuffer = VulkanBuffer(data, size, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        
        m_vertexBuffer.Setup(size, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

        // copy data from staging buffer to device local final vertex buffer
        VulkanBuffer::CopyTo(&stagingBuffer, &m_vertexBuffer, size);
    }

    void VulkanVertexBuffer::Bind() const
    {
        // no op in Vulkan,
    }

    void VulkanVertexBuffer::Unbind() const
    {
        // no op in vulkan,
    }

    void VulkanVertexBuffer::SetData(const void* data, uint64_t size)
    {
        m_vertexBuffer.Flush();
        m_vertexBuffer.Unmap();
        m_vertexBuffer.Map();
        m_vertexBuffer.SetData(&data, size);
        m_vertexBuffer.Unmap();
    }

    void VulkanVertexBuffer::SetLayout(const VX::BufferLayout& layout)
    {
//        m_Layout = layout; // really don't know why this line gives error
        
        int stride = 0;
        int offset = 0;
        
        int locationIndex = 0;
        
        for(const auto& element: layout.Elements)
        {
            VkVertexInputAttributeDescription attribute;
            attribute.binding = 0;
            attribute.location = locationIndex;
            attribute.offset = offset;
            attribute.format = ShaderDataTypeToVulkanFormat(element.Type);
//            VX_CORE_INFO("{0}: offset {1}", element.Name, attribute.offset);
            m_attributeDescrtiptions.push_back(attribute);
            
             offset += ShaderDataTypeSize(element.Type);
             stride += ShaderDataTypeSize(element.Type);
            locationIndex++;
        }
        
        m_bindingDescription.binding = 0;
        m_bindingDescription.stride = stride; // number of bytes from one entry to the next
//        VX_CORE_INFO("Stride: {0}", stride);
        m_bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // per-vertex data, TODO: change this for particle system
    }

    VulkanIndexBuffer::VulkanIndexBuffer(void* data, VkDeviceSize size)
        :VulkanBuffer(size, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
    {
        VulkanBuffer stagingBuffer = VulkanBuffer(data, size, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        
        m_cmdManager->CopyBuffer(stagingBuffer.Buffer, this->Buffer, size);
    }
}
