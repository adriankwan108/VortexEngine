#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"
#include <glm/glm.hpp>

#include "Renderer/Buffer.hpp"

#include "VulkanInitializer.hpp"
#include "VulkanTools.hpp"
#include "VulkanDevice.hpp"
#include "VulkanBufferLayout.hpp"

namespace vkclass
{
    class VulkanBuffer
    {
    public:
        VulkanBuffer(VkDeviceSize size, VkBufferUsageFlags flags); // bind, map... explicitly
        VulkanBuffer(void* data, VkDeviceSize size, VkBufferUsageFlags flags); // map implicitly
        ~VulkanBuffer();
        
        const VkBuffer& Buffer = m_buffer;
        // get memory()
        // get size
        // get data
        
        void Bind(VkDeviceSize offset = 0);
        void Map();
        void Unmap();
        void SetData(void* data, VkDeviceSize size);
        void Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
        
//        void update();
        
        static void Init(vkclass::VulkanDevice* device);
    private:
        // status
//        bool mapped = false;
//        bool persistent = false;
        
        static vkclass::VulkanDevice* m_device;
        
        // refs
        VkDeviceSize m_size = 0; // size in bytes of the buffer
        // usage fklags
        // allocation flags
        // queue family
        
        // props
        VkBuffer m_buffer = VK_NULL_HANDLE;
        VkDeviceMemory m_memory = VK_NULL_HANDLE;
        void* m_data;
    };

    class VulkanVertexBuffer : public VulkanBuffer, public VX::VertexBuffer
    {
    public:
        VulkanVertexBuffer(VkDeviceSize size, VkBufferUsageFlags flags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
            : VulkanBuffer(size, flags)
        {
        }
        VulkanVertexBuffer(void* data, VkDeviceSize size, VkBufferUsageFlags flags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
            : VulkanBuffer(data, size, flags)
        {
        }
        
        const vkclass::VulkanBufferLayout& GetLayout() const { return m_Layout; }
        
        // transform general buffer layout to vulkan buffer layout implicitly
        virtual void SetLayout(const VX::BufferLayout& layout) override { m_Layout = layout; }
    private:
        vkclass::VulkanBufferLayout m_Layout;
    };

    class VulkanIndexBuffer
    {
        
    };

}
