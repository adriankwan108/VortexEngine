#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"
#include <glm/glm.hpp>

#include "Renderer/Buffer.hpp"

#include "VulkanInitializer.hpp"
#include "VulkanTools.hpp"
#include "VulkanDevice.hpp"

#include "VulkanCommandManager.hpp"

namespace vkclass
{
    /*
     *  Noted that we're not supposed to actually call vkAllocateMemory for every individual buffer.
     *  The maximum number of simulaneous memory allocations is limited by maxMemoryAllocationCount,
     *  low as 4096 even on highend hardware.
     *
     *  TODO: allocate memory for a large number of objects at the same time, to create a custom allocator that splits up a single allocation among many different objects by using the offset
     */
    class VulkanBuffer
    {
    public:
        VulkanBuffer() = default;
        VulkanBuffer(VkDeviceSize size, VkMemoryPropertyFlags props,VkBufferUsageFlags usage); // bind, map... explicitly
        VulkanBuffer(void* data, VkDeviceSize size, VkMemoryPropertyFlags props, VkBufferUsageFlags usage); // map implicitly
        virtual ~VulkanBuffer();
        
        const VkBuffer& Buffer = m_buffer;
        // get memory()
        // get size
        // get data
       
        // this is a delay construction
        void Setup(VkDeviceSize size, VkMemoryPropertyFlags props,VkBufferUsageFlags usage);
        
        void Bind(VkDeviceSize offset = 0);
        void Map();
        void Unmap();
        void SetData(void* data, VkDeviceSize size);
        void Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
        
//        void update();
        
        static void Init(vkclass::VulkanDevice* device, vkclass::VulkanCommandManager* cmdManager);
        static void CopyTo(VulkanBuffer* srcBuffer, VulkanBuffer* dstBuffer, VkDeviceSize size);
    protected:
        // status
//        bool mapped = false;
//        bool persistent = false;
        
        static vkclass::VulkanDevice* m_device;
        static vkclass::VulkanCommandManager* m_cmdManager;
        
        // refs
        VkDeviceSize m_size = 0; // size in bytes of the buffer
        // usage fklags
        // allocation flags
        // queue family
        
        // props
        VkBuffer m_buffer = VK_NULL_HANDLE;
        VkDeviceMemory m_memory = VK_NULL_HANDLE;
        void* m_data = nullptr;
        
    private:
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage);
        void createMemory(VkMemoryPropertyFlags props);
    };

    class VulkanVertexBuffer : public VX::VertexBuffer
    {
    public:
        VulkanVertexBuffer(void* data, VkDeviceSize size);
        virtual ~VulkanVertexBuffer() = default;
        
        virtual void Bind() const override;
        virtual void Unbind() const override;
        
        virtual void SetData(const void* data, uint64_t size) override;

        virtual void SetLayout(const VX::BufferLayout& layout) override;
        virtual const VX::BufferLayout& GetLayout() const override { return m_Layout; }
        
    private:
        VulkanBuffer m_vertexBuffer; // optimized, device local final vertex buffer
        VX::BufferLayout m_Layout;
    };

    class VulkanIndexBuffer : public VulkanBuffer
    {
    public:
        // optimized, device local final vertex buffer
        VulkanIndexBuffer(void* data, VkDeviceSize size);
    };

}
