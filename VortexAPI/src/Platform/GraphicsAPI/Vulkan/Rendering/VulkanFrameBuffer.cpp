#include "VulkanFramebuffer.hpp"
#include "VortexPCH.hpp"
#include "Core/VulkanTools.hpp"

namespace vkclass
{
    VulkanFramebuffer::VulkanFramebuffer(VkDevice device, VkRenderPass renderPass, const std::vector<VkImageView>& imageViews, const VkExtent2D& extent)
        :m_device(device)
    {
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;

        framebufferInfo.attachmentCount = static_cast<uint32_t>(imageViews.size());
        framebufferInfo.pAttachments = imageViews.data();
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        VK_CHECK_RESULT(vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_framebuffer));
    }

    VulkanFramebuffer::~VulkanFramebuffer()
    {
        if (m_framebuffer != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(m_device, m_framebuffer, nullptr);
            VX_CORE_TRACE("VulkanFramebuffer destroyed.");
        }
    }
}