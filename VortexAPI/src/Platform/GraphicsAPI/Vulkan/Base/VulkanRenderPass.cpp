#include "VulkanRenderPass.hpp"

namespace vkclass
{
    VulkanRenderPass::VulkanRenderPass(VkDevice device): m_device(device)
    {
        
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        vkDestroyRenderPass(m_device, m_renderPass, nullptr);
        VX_CORE_INFO("VulkanRenderPass: PipelineLayout destroyed");
        VX_CORE_INFO("VulkanRenderPass: RenderPass destroyed");
    }

    void VulkanRenderPass::AddSubpass(vkclass::VulkanSubpass subpass)
    {
        m_subpasses.push_back(subpass.Subpass);
        for(const auto& attachment: subpass.Attachments)
        {
            m_attachments.push_back(attachment.description);
        }
    }

    void VulkanRenderPass::AddDependency()
    {
        // as only one subpass for now
        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL; // implicit subpass before or after the render pass depending on whether it is specified in srcSubpass or dstSubpass
        dependency.dstSubpass = 0; // refers to the first subpass
        
        // wait on operation stages
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // wait for swapchain finish reading image
        dependency.srcAccessMask = 0;
        
        // prevent the transition from happening until it's actually necessary
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        
        m_dependencies.push_back(dependency);
    }

    void VulkanRenderPass::Create()
    {
        VkRenderPassCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        // attachments
        createInfo.attachmentCount = static_cast<uint32_t>(m_attachments.size());
        createInfo.pAttachments = m_attachments.data();
        VX_CORE_INFO("Render Pass: attachment count: {0}", m_attachments.size());
        // subpasses
        createInfo.subpassCount = static_cast<uint32_t>(m_subpasses.size());
        createInfo.pSubpasses = m_subpasses.data();
        
        createInfo.dependencyCount = static_cast<uint32_t>(m_dependencies.size());
        createInfo.pDependencies = m_dependencies.data();
        
        VK_CHECK_RESULT(vkCreateRenderPass(m_device, &createInfo, nullptr, &m_renderPass));
    }
}
