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
        
        virtual void AddVertexBuffer(std::shared_ptr<VX::VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(std::shared_ptr<VX::IndexBuffer>& indexBuffer) override;
//        virtual void SetLayout(const BufferLayout layout) override;
        
    private:
        std::vector<std::shared_ptr<VX::VertexBuffer>> m_VertexBuffers;
        std::vector<std::shared_ptr<VX::IndexBuffer>> m_IndexBuffers;
        
    };
}
