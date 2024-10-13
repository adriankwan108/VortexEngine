#pragma once
#include "vulkan/vulkan.h"
#include "Renderer/RenderTarget.hpp"
#include "VulkanSwapChain.hpp"
#include "VulkanRenderPass.hpp"

namespace vkclass
{
    class VulkanRenderTarget;

    /*
     This manages all abstrated framebuffer objects (frames in flight) and render passes
     */
    class VulkanRenderTargetManager
    {
    public:
        VulkanRenderTargetManager(VkDevice device, VulkanSwapChain* swapchain);
        ~VulkanRenderTargetManager();
        
        void CreateSwapChainTarget();
        VulkanRenderPass* GetRenderPass(const std::string& name);

    private:
        VkDevice m_device;
        VulkanSwapChain* m_swapchain;
        std::unordered_map<std::string, VX::Scope<VulkanRenderPass>> m_RenderPasses;
        // map of <name, framebuffers>
        
        // get or create renderpass
        // get or create framebuffer
    };


    class VulkanRenderTarget : public VX::RenderTarget
    {
    public:
        VulkanRenderTarget(const std::string& name, const VX::RenderTargetSpecification& spec);
        ~VulkanRenderTarget();

        virtual void Bind()     override;
        virtual void Unbind()   override;

    public:
        static void Init(VulkanRenderTargetManager* manager);
    
    private:
        friend class VulkanRenderTargetManager;
        static inline VulkanRenderTargetManager* s_manager = nullptr;
        
        std::string m_name;
        VX::RenderTargetSpecification m_spec;
        
        VulkanRenderPass* m_RenderPass = nullptr;
        // reference of framebuffer
    };
}
