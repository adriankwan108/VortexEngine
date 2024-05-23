#pragma once
#include "VortexPCH.hpp"

#include <memory>
#include <vector>
#include "vulkan/vulkan.h"

#include "Core/VulkanTools.hpp"
#include "Core/VulkanBuffer.hpp"
#include "Core/VulkanCommandManager.hpp"

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
        
        // TODO: revise this as this is not in used ?
        virtual const std::vector<VX::Ref<VX::VertexBuffer>>& GetVertexBuffer() const override { return m_VertexBuffers; }
        virtual const VX::Ref<VX::IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
        
    public:
        static void Init(vkclass::VulkanCommandManager* commandBufferManager);
        
    private:
        std::vector<VX::Ref<VX::VertexBuffer>> m_VertexBuffers;
        VX::Ref<VX::IndexBuffer> m_IndexBuffer;

        static vkclass::VulkanCommandManager* s_commandBufferManager;
        
    private:
        std::vector<VkBuffer> m_VkVertexBuffers;
        std::vector<VkDeviceSize> m_VkVBsOffsets;
        uint32_t m_LastStride = 0;
        
        VkBuffer m_VkIndexBuffer;
    };
}
