#include "VortexPCH.hpp"
#include "Renderer/RendererAPI.hpp"

#include "vulkan/vulkan.h"
#include "Core/VulkanCommandManager.hpp"

namespace vkclass
{
    class VulkanRendererAPI : public VX::RendererAPI
    {
    public:        
        virtual void Init() override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void DrawIndexed(const VX::Ref<VX::VertexArray>& vertexArray) override;
        virtual void Clear() override;
        
    public:
        // below function called when VulkanContext created
        // as init should only pass renderer agnositc configs, api-related settings should be hiden
        static void SetCommandManager(vkclass::VulkanCommandManager* commandManager);
        
    private:
        static vkclass::VulkanCommandManager* s_CommandManager;
    };
}
