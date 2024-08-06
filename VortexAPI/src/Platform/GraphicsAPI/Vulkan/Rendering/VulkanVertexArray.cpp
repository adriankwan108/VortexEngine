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

        s_commandBufferManager->BindVertexBuffer(m_vkVertexBuffers, m_vkOffsets);
        s_commandBufferManager->BindIndexBuffer(m_vkIndexBuffer);
    }

    void VulkanVertexArray::Unbind() const
    {
        
    }

    void VulkanVertexArray::Init(vkclass::VulkanCommandManager* commandBufferManager)
    {
        s_commandBufferManager = commandBufferManager;
    }

    void VulkanVertexArray::AddApiVertexBuffer(VX::Ref<VX::VertexBuffer>& vertexBuffer)
    {
        auto vkVB = std::static_pointer_cast<VulkanVertexBuffer>(vertexBuffer);
        m_vkVertexBuffers.push_back(vkVB->GetBuffer());
        m_vkOffsets.push_back(m_lastStride);
        m_lastStride += vkVB->GetStride();
    }

    void VulkanVertexArray::SetApiIndexBuffer(VX::Ref<VX::IndexBuffer>& indexBuffer)
    {
        auto vkIB = std::static_pointer_cast<VulkanIndexBuffer>(m_indexBuffer);
        m_vkIndexBuffer = vkIB->GetBuffer();
    }
}
