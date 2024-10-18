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
        m_FrameBuffersMap.clear();
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
        
        // WARN: huge byte usage of stack in this line, not sure the reason
        VulkanRenderPassSpecification renderPassSpec(VX::SubpassHint::None, { colorAttachment } );

        // get spec hash
        // m_RenderPasses.insert(std::make_pair(hash, renderpass));
        
        m_RenderPasses.insert(std::make_pair("swapchain", VX::CreateScope<VulkanRenderPass>(m_device, renderPassSpec)));

        m_FrameBuffersMap["swapchain"].reserve(m_swapchain->GetImageCount());
        for (size_t i = 0; i < m_swapchain->GetImageCount(); i++)
        {
            std::vector<VkImageView> attachments = {
                m_swapchain->GetImageBuffers()[i].view
            };

            m_FrameBuffersMap["swapchain"].emplace_back(
                VX::CreateScope<VulkanFramebuffer>(m_device, m_RenderPasses["swapchain"]->GetHandle(), attachments, m_swapchain->GetExtent())
            );
        }
    }

    VulkanRenderPass* VulkanRenderTargetManager::GetOrCreateRenderPass(const std::string& hash, const VX::RenderTargetSpecification& spec) const
    {
        auto it = m_RenderPasses.find(hash);

        if (it != m_RenderPasses.end())
        {
            return it->second.get();
        }
        else
        {
            // TODO: create render pass
            VX_CORE_WARN("VulkanRenderTargetManager::Create RenderPass is not yet implemented.");
            return nullptr;
        }
    }

    std::vector<VulkanFramebuffer*> VulkanRenderTargetManager::GetOrCreateFramebuffer(const std::string& hash) const
    {
        std::vector<VulkanFramebuffer*> res;

        auto it = m_FrameBuffersMap.find(hash);
        if (it != m_FrameBuffersMap.end()) 
        {
            for (auto& framebuffer : it->second)
            {
                res.emplace_back(framebuffer.get());
            }
        }
        else
        {
            // TODO: create framebuffer
            VX_CORE_WARN("VulkanRenderTargetManager::Create framebuffer is not yet implemented.");
        }
        return res;
    }

    void VulkanRenderTargetManager::SetFramebufferIndex(uint32_t* framebufferIndex)
    {
        VX_CORE_INFO("swapchain available: {0}", m_swapchain->m_availableImageIndex);
        framebufferIndex = &(m_swapchain->m_availableImageIndex);
    }


    VulkanRenderTarget::VulkanRenderTarget(const std::string& name, const VX::RenderTargetSpecification& spec)
        :m_name(name), m_spec(spec)
    {
        // TODO:
        // spec => Vulkan's RTSpec => GetHash();

        if (spec.IsSwapChainTarget)
        {
            m_RenderPass = s_manager->GetOrCreateRenderPass("swapchain", spec);
            m_framebuffers = s_manager->GetOrCreateFramebuffer("swapchain");
            s_manager->SetFramebufferIndex(m_framebufferIndex);
            if (m_framebufferIndex)
            {
                VX_CORE_TRACE("{0}", *m_framebufferIndex);
            }
            else
            {
                VX_CORE_TRACE("nullptr");
            }
        }
        else
        {
            m_framebufferIndex = new uint32_t(0);
            VX_CORE_WARN("VulkanRenderTarget:: Creation of non swap chain target is not supported yet.");
        }
    }

    VulkanRenderTarget::~VulkanRenderTarget()
    {
        delete m_framebufferIndex;
        m_framebufferIndex = nullptr;
        // TODO:
        // clear ptr of framebuffer and renderpass
        // request the manager to clear the reference count of framebuffer and renderpass
    }

    void VulkanRenderTarget::Init(VulkanRenderTargetManager* manager)
    {
        s_manager = manager;
    }

    void VulkanRenderTarget::Bind()
    {
        // VX_CORE_TRACE("VulkanRenderTarget: framebuffers: {0}", m_framebuffers.size());
        if (m_framebufferIndex)
        {
            VX_CORE_TRACE("{0}", *m_framebufferIndex);
        }
        else
        {
            VX_CORE_TRACE("nullptr");
        }
        // bind renderpass and framebuffer
    }

    void VulkanRenderTarget::Unbind()
    {
        // unbind renderpass
    }
}
