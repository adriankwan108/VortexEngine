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

    void VulkanVertexArray::AddVertexBuffer(VX::Ref<VX::VertexBuffer>& vertexBuffer)
    {
        vertexBuffer->Bind();
        m_VertexBuffers.push_back(vertexBuffer);
        
        auto vkVB = std::static_pointer_cast<VulkanVertexBuffer>(vertexBuffer);
        m_VkVertexBuffers.push_back(vkVB->GetBuffer());
        
        m_VkVBsOffsets.push_back(static_cast<VkDeviceSize>(m_LastStride));
        m_LastStride += vkVB->GetLayout().GetStride();
    }

    void VulkanVertexArray::SetIndexBuffer(VX::Ref<VX::IndexBuffer>& indexBuffer)
    {
        m_IndexBuffer = indexBuffer;
        m_VkIndexBuffer = std::static_pointer_cast<VulkanIndexBuffer>(m_IndexBuffer)->GetBuffer();
    }

    uint32_t VulkanVertexArray::GetIndicesSize()
    {
        return m_IndexBuffer->GetCount();
    }
}
