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
        // swapchain format
        // swapchain store op
        // swapchain load op
        
        // attachment spec
        
        // transform to renderpass spec
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = m_swapchain->GetSurfaceFormat().format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        
        VulkanRenderPassSpecification renderPassSpec(VX::SubpassHint::None, {colorAttachment});
        // get spec hash
        // m_RenderPasses.insert(std::make_pair(hash, renderpass));
        
        m_RenderPasses.insert(std::make_pair("swapchain", VX::CreateScope<VulkanRenderPass>(m_device, renderPassSpec)));
    }

    VulkanRenderPass* VulkanRenderTargetManager::GetRenderPass(const std::string& name)
    {
        auto it = m_RenderPasses.find(name);
        if (it != m_RenderPasses.end())
        {
            return it->second.get();
        }
        else
        {
            return nullptr;
        }
    }


    VulkanRenderTarget::VulkanRenderTarget(const std::string& name, const VX::RenderTargetSpecification& spec)
        :m_name(name), m_spec(spec)
    {
        if (m_spec.IsSwapChainTarget)
        {
            m_RenderPass = s_manager->GetRenderPass("swapchain");
        }
        else
        {
            VX_CORE_INFO("VulkanRenderTarget:: Non swapchain target is not supported yet.");
        }
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
