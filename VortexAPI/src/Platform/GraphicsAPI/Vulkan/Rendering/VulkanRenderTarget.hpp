#pragma once
#include "vulkan/vulkan.h"
#include "Renderer/RenderTarget.hpp"

namespace vkclass
{
    class VulkanRenderTarget;

    class VulkanRenderTargetManager
    {
    public:
        // GetOrCreateRenderPass();
        // GetOrCreateFramebuffer();

    private:
        std::unordered_map<std::string, VulkanRenderTarget> m_map; // TODO: examine if hash map is better
    };

    class VulkanRenderTarget : public VX::RenderTarget
    {
    public:
        VulkanRenderTarget(const VX::RenderTargetSpecification& spec);
        ~VulkanRenderTarget();

        virtual void Bind()     override;
        virtual void Unbind()   override;

    public:
        static void Init(VulkanRenderTargetManager* managerr);
    
    private:
        static inline VulkanRenderTargetManager* s_manager = nullptr;
        // VkRenderPass* m_renderPass;
        // VkFramebuffer* m_framebuffer;
    };
}