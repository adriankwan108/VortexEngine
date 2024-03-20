#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"
#include "VulkanInitializer.hpp"
#include "VulkanTools.hpp"
#include "VulkanDevice.hpp"

namespace vkclass
{
    
    class VulkanBuffer
    {
    public:
        VulkanBuffer(VkDeviceSize size, VkBufferUsageFlags flags); // prop flags ?
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

    class VulkanVertexBuffer
    {
        
    };

    class VulkanIndexBuffer
    {
        
    };
}
