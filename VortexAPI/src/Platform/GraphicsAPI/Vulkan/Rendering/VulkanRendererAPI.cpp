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

    void VulkanRendererAPI::DrawIndexed(const VX::Ref<VX::VertexArray> &vertexArray)
    {
        auto drawInfoList = vertexArray->GetIndexBuffer()->GetDrawCmdInfoList();
        
        for(auto& info : drawInfoList)
        {
            s_CommandManager->DrawIndexed(info.indexCount, info.firstIndex, info.vertexOffset);
        }
    }

    void VulkanRendererAPI::Clear()
    {
        
    }

    void VulkanRendererAPI::SetCommandManager(vkclass::VulkanCommandManager* commandManager)
    {
        s_CommandManager = commandManager;
    }

}
