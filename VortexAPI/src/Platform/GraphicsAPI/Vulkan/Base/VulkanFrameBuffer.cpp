#include "VulkanFrameBuffer.hpp"

namespace vkclass
{
    VulkanFrameBuffer::VulkanFrameBuffer(vkclass::VulkanDevice* device, vkclass::VulkanSwapChain* swapChain):
        m_device(device), m_swapChain(swapChain)
    {
        m_device = device;
        
        Width = m_swapChain->Extent.width;
        Height = m_swapChain->Extent.height;
        
        m_extent.width = Width;
        m_extent.height = Height;
    }

    VulkanFrameBuffer::~VulkanFrameBuffer()
    {
        vkDestroyFramebuffer(m_device->LogicalDevice, m_frameBuffer, nullptr);
        VX_CORE_INFO("VulkanFrameBuffer: FrameBuffer destroyed.");
    }

    void VulkanFrameBuffer::AddRenderPass(vkclass::VulkanRenderPass* renderPass)
    {
        m_renderPass = renderPass->RenderPass;
    }

    void VulkanFrameBuffer::SetUpFrameBuffer(std::vector<VkImageView> imageViews)
    {
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.pAttachments = imageViews.data();
         framebufferInfo.attachmentCount = static_cast<uint32_t>(imageViews.size());
//        framebufferInfo.attachmentCount = 1;
        framebufferInfo.width = Width;
        framebufferInfo.height = Height;
        framebufferInfo.layers = 1;
//        framebufferInfo.layers = maxLayers;
        VK_CHECK_RESULT(vkCreateFramebuffer(m_device->LogicalDevice, &framebufferInfo, nullptr, &m_frameBuffer));
    }
}
