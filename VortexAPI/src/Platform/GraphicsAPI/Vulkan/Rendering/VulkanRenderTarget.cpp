#include "VortexPCH.hpp"
#include "VulkanRenderTarget.hpp"

namespace vkclass
{
    VulkanRenderTargetManager::VulkanRenderTargetManager()
    {
        
    }

    VulkanRenderTargetManager::~VulkanRenderTargetManager()
    {
        
    }

    void VulkanRenderTargetManager::PrepareTarget(VulkanRenderTarget* target)
    {
        VX_CORE_TRACE("VulkanRenderTargetManager::Preparing for {0}", target->m_name);
        if(target->m_spec.IsSwapChainTarget)
        {
            VX_CORE_TRACE("VulkanRenderTargetManager:: this is swap chain target");

        }
    }


    VulkanRenderTarget::VulkanRenderTarget(const std::string& name, const VX::RenderTargetSpecification& spec)
        :m_name(name), m_spec(spec)
    {
        s_manager->PrepareTarget(this);
    }

    VulkanRenderTarget::~VulkanRenderTarget()
    {
        // clear ptr of framebuffer and renderpass
        // request the manager to clear the reference count of framebuffer and renderpass
    }

    void VulkanRenderTarget::Init(VulkanRenderTargetManager* manager)
    {
        s_manager = manager;
    }

    void VulkanRenderTarget::Bind()
    {
        // bind renderpass and framebuffer
    }

    void VulkanRenderTarget::Unbind()
    {
        // unbind renderpass
    }
}
