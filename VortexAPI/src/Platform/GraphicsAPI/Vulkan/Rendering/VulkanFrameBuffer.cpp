#include "VulkanFrameBuffer.hpp"

namespace vkclass
{
    VulkanFrameBuffer::VulkanFrameBuffer(vkclass::VulkanDevice* device, uint32_t width, uint32_t height):
        m_device(device)
    {
        
        m_extent.width = width;
        m_extent.height = height;
    }

    VulkanFrameBuffer::~VulkanFrameBuffer()
    {
        vkDestroyFramebuffer(m_device->LogicalDevice, m_frameBuffer, nullptr);
        VX_CORE_INFO("VulkanFrameBuffer: FrameBuffer destroyed.");
    }

    void VulkanFrameBuffer::SetRenderPass(const VkRenderPass& renderPass)
    {
        if(renderPass ==  VK_NULL_HANDLE)
        {
            VX_CORE_ERROR("VulkanFrameBuffer::Set null renderpass.");
            std::runtime_error("VulkanFrameBuffer::Set null renderpass.");
        }
        m_renderPass = renderPass;
    }

    void VulkanFrameBuffer::SetUpFrameBuffer(std::vector<VkImageView> imageViews)
    {
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.pAttachments = imageViews.data();
         framebufferInfo.attachmentCount = static_cast<uint32_t>(imageViews.size());
//        framebufferInfo.attachmentCount = 1;
        framebufferInfo.width = m_extent.width;
        framebufferInfo.height = m_extent.height;
        framebufferInfo.layers = 1;
//        framebufferInfo.layers = maxLayers;
        VK_CHECK_RESULT(vkCreateFramebuffer(m_device->LogicalDevice, &framebufferInfo, nullptr, &m_frameBuffer));
    }
}
