#include "VulkanRendererAPI.hpp"
#include "VulkanVertexArray.hpp"

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
        auto va = std::static_pointer_cast<VulkanVertexArray>(vertexArray);
        
        s_CommandManager->BindVertexBuffer(va->GetVertexBuffers(), va->GetVBsOffsets());
        s_CommandManager->BindIndexBuffer(va->GetIndexBuffer());
        
        s_CommandManager->Draw(va->GetIndicesSize());
    }

    void VulkanRendererAPI::Clear()
    {
        
    }

    void VulkanRendererAPI::SetCommandManager(vkclass::VulkanCommandManager* commandManager)
    {
        s_CommandManager = commandManager;
    }

}
