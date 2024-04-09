#include "VulkanRendererAPI.hpp"

namespace vkclass
{
    vkclass::VulkanCommandManager* VulkanRendererAPI::s_CommandManager = nullptr;

    void VulkanRendererAPI::Init()
    {
        // get reference
        
        // renderer configs
        // enable depth test...
    }

    void VulkanRendererAPI::SetClearColor(const glm::vec4 &color)
    {
        s_CommandManager->SetClearColor(color);
    }

    void VulkanRendererAPI::DrawIndexed(const std::shared_ptr<VX::VertexArray> &vertexArray)
    {
        // m_CommandManager.BindVertexBuffer({vertexBuffer->Buffer}, {0});
        // m_CommandManager.BindIndexBuffer(indexBuffer->Buffer, 0);
        //
        // m_CommandManager.Draw(static_cast<uint32_t>(triangleIndices.size()));
    }

    void VulkanRendererAPI::Clear()
    {
        
    }

    void VulkanRendererAPI::SetCommandManager(vkclass::VulkanCommandManager* commandManager)
    {
        s_CommandManager = commandManager;
    }

}
