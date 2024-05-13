#pragma once
#include "VortexPCH.hpp"

#include <memory>
#include <vector>
#include "vulkan/vulkan.h"

#include "Core/VulkanTools.hpp"
#include "Core/VulkanBuffer.hpp"

#include "Renderer/VertexArray.hpp"

namespace vkclass
{
    class VulkanVertexArray : public VX::VertexArray
    {
    public:
        VulkanVertexArray();
        virtual ~VulkanVertexArray();
        
        virtual void Bind() const override;
        virtual void Unbind() const override;
        
        virtual void AddVertexBuffer(VX::Ref<VX::VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer (VX::Ref<VX::IndexBuffer>& indexBuffer) override;
        
        virtual const std::vector<VX::Ref<VX::VertexBuffer>>& GetVertexBuffer() const override { return m_VertexBuffers; }
        virtual const VX::Ref<VX::IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
        
    public:
        std::vector<VkBuffer> GetVertexBuffers() { return m_VkVertexBuffers; }
        std::vector<VkDeviceSize> GetVBsOffsets() { return m_VkVBsOffsets; }
        VkBuffer GetIndexBuffer() { return m_VkIndexBuffer; }
        uint32_t GetIndicesSize();
        
    private:
        std::vector<VX::Ref<VX::VertexBuffer>> m_VertexBuffers;
        VX::Ref<VX::IndexBuffer> m_IndexBuffer;
        
    private:
        std::vector<VkBuffer> m_VkVertexBuffers;
        std::vector<VkDeviceSize> m_VkVBsOffsets;
        uint32_t m_LastStride = 0;
        
        VkBuffer m_VkIndexBuffer;
    };
}
