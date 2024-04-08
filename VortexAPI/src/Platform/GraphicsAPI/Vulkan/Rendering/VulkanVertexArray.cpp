#include "VulkanVertexArray.hpp"

namespace vkclass
{
    VulkanVertexArray::VulkanVertexArray()
    {
        // glcreatevertexarray(1, &rendererid)
    }

    VulkanVertexArray::~VulkanVertexArray()
    {
        
    }

    void VulkanVertexArray::Bind() const
    {
        // OpenGL: Binding VAO, each vbo bind in their related buffer
        // Vulkan: No VAO => no operations, But binding VBOs to command buffers in the caller
    }

    void VulkanVertexArray::Unbind() const
    {
        
    }

    void VulkanVertexArray::AddVertexBuffer(std::shared_ptr<VX::VertexBuffer>& vertexBuffer)
    {
        vertexBuffer->Bind();
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void VulkanVertexArray::SetIndexBuffer(std::shared_ptr<VX::IndexBuffer>& indexBuffer)
    {
        m_IndexBuffer = indexBuffer;
    }
}
