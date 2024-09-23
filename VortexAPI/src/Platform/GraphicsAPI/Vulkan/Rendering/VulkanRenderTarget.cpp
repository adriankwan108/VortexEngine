#include "VortexPCH.hpp"
#include "VulkanRenderTarget.hpp"

namespace vkclass
{
    VulkanRenderTargetManager::VulkanRenderTargetManager(VkDevice device, VulkanSwapChain* swapchain)
        :m_device(device), m_swapchain(swapchain)
    {
        
    }

    VulkanRenderTargetManager::~VulkanRenderTargetManager()
    {
        VX_CORE_TRACE("VulkanRenderTargetManager destroyed.");
        m_RenderPasses.clear();
    }

    void VulkanRenderTargetManager::CreateSwapChainTarget()
    {
        // get spec from swapchain
        
        // create render pass for swapchain
        VulkanRenderPassBuilder builder(m_device);
        m_RenderPasses.insert(std::make_pair("swapchain", builder.Create()));
    }

    void VulkanRenderTargetManager::PrepareTarget(VulkanRenderTarget* target)
    {
        VX_CORE_TRACE("VulkanRenderTargetManager::Preparing for {0}", target->m_name);
        if(target->m_spec.IsSwapChainTarget)
        {
            // get the reference of swapchain's renderpass and framebuffer
        }else
        {
            // get or create renderpass and framebuffer
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
