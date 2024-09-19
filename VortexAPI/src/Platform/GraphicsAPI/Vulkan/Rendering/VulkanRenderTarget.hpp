#pragma once
#include "vulkan/vulkan.h"
#include "Renderer/RenderTarget.hpp"

namespace vkclass
{
    class VulkanRenderTarget;

    /*
     This manages all abstrated framebuffer objects (frames in flight) and render passes
     */
    class VulkanRenderTargetManager
    {
    public:
        VulkanRenderTargetManager();
        ~VulkanRenderTargetManager();
        
        void PrepareTarget(VulkanRenderTarget* target);

    private:
        // map of <name, renderpasses>
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
    };
}
