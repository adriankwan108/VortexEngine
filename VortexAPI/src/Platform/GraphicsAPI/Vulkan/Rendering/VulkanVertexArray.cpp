#include "VulkanVertexArray.hpp"

namespace vkclass
{
    vkclass::VulkanCommandManager* VulkanVertexArray::s_commandBufferManager = nullptr;

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
        // Vulkan: No VAO, but bind all vbos here

        s_commandBufferManager->BindVertexBuffer(m_VkVertexBuffers, m_VkVBsOffsets);
        s_commandBufferManager->BindIndexBuffer(m_VkIndexBuffer);
    }

    void VulkanVertexArray::Unbind() const
    {
        
    }

    void VulkanVertexArray::Init(vkclass::VulkanCommandManager* commandBufferManager)
    {
        s_commandBufferManager = commandBufferManager;
    }

    void VulkanVertexArray::AddVertexBuffer(VX::Ref<VX::VertexBuffer>& vertexBuffer)
    {
        // vertexBuffer->Bind(); // used by opengl
        m_VertexBuffers.push_back(vertexBuffer);
        
        /*auto vkVB = std::static_pointer_cast<VulkanVertexBuffer>(vertexBuffer);
        m_VkVertexBuffers.push_back(vkVB->GetBuffer());
        
        m_VkVBsOffsets.push_back(static_cast<VkDeviceSize>(m_LastStride));
        m_LastStride += vkVB->GetLayout().GetStride();*/
    }

    void VulkanVertexArray::SetIndexBuffer(VX::Ref<VX::IndexBuffer>& indexBuffer)
    {
        m_IndexBuffer = indexBuffer;
        m_VkIndexBuffer = std::static_pointer_cast<VulkanIndexBuffer>(m_IndexBuffer)->GetBuffer();
    }
}
