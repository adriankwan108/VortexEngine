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
     *  Noted that it is not supposed to call vkAllocateMemory for every individual buffer.
     *  The maximum number of simulaneous memory allocations is limited by maxMemoryAllocationCount,
     *  which it is 4096 even on highend hardware, pretty low.
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
        VkDescriptorBufferInfo& GetDescriptorBufferInfo() { return m_bufferInfo; }
        // get memory()
        // get size
        // get data
       
        // this is a delay construction
        void Setup(VkDeviceSize size, VkMemoryPropertyFlags props,VkBufferUsageFlags usage);
        
        void Bind(VkDeviceSize offset = 0); // binding to memory
        void Map();
        void Unmap();
        void SetData(void* data, VkDeviceSize size);
        void Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
        
        static void Init(vkclass::VulkanDevice* device, vkclass::VulkanCommandManager* cmdManager);
        static void CopyTo(VulkanBuffer* srcBuffer, VulkanBuffer* dstBuffer, VkDeviceSize size);
        
        static int GetMaxFrameInFlight() { return m_cmdManager->GetMaxFrameInFlight(); }
        static uint32_t GetCurrentFrame() { return m_cmdManager->GetCurrentFrame(); }
        
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
        
        VkDescriptorBufferInfo m_bufferInfo{};
        void* m_data = nullptr;
        
    private:
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage);
        void createMemory(VkMemoryPropertyFlags props);
    };

    class VulkanVertexBuffer : public VX::VertexBuffer
    {
    public:
        //VulkanVertexBuffer() = default;
        explicit VulkanVertexBuffer(void* data, VkDeviceSize totalSize, uint64_t stride);
        virtual ~VulkanVertexBuffer() = default;
        
        virtual void Bind() const override;
        virtual void Unbind() const override;
        
        const VkBuffer& GetBuffer() const { return m_vertexBuffer.Buffer; }
        
    private:
        VulkanBuffer m_vertexBuffer; // optimized, device local final vertex buffer
    };

    class VulkanIndexBuffer : public VX::IndexBuffer
    {
    public:
        explicit VulkanIndexBuffer(void* data, VkDeviceSize size);
        virtual ~VulkanIndexBuffer() = default;

        virtual void Bind() const override;
        virtual void Unbind() const override;
        
        virtual void SetData(void* data, uint64_t size) override;
        
    public:
        const VkBuffer& GetBuffer() const { return m_indexBuffer.Buffer ;}

    private:
        VulkanBuffer m_indexBuffer; // optimized, device local final index buffer
    };

    /*
     *  Implicitly create uniform buffers for max frames in flight
     */
    class VulkanUniformBuffer : public VX::UniformBuffer
    {
    public:
        VulkanUniformBuffer(VkDeviceSize size);
        virtual ~VulkanUniformBuffer() = default;
        
        virtual void Update(void* data, uint64_t size) override;
        
        std::vector<VkDescriptorBufferInfo*> GetBuffersInfo();
    private:
        std::vector<VulkanBuffer> m_UniformBuffers;
    };

}
