#include "VulkanFrameBuffer.hpp"

namespace vkclass
{
    VulkanFrameBuffer::VulkanFrameBuffer(vkclass::VulkanDevice* device, vkclass::VulkanSwapChain* swapChain):
        m_device(device), m_swapChain(swapChain)
    {
        m_device = device;
        
        Width = m_swapChain->Extent.width;
        Height = m_swapChain->Extent.height;
    }

    VulkanFrameBuffer::~VulkanFrameBuffer()
    {
//        for(auto attachment: m_attachments)
//        {
//
//        }
        vkDestroyRenderPass(m_device->LogicalDevice, m_renderPass, nullptr);
        VX_CORE_INFO("VulkanFrameBuffer: RenderPass destroyed.");
        
        vkDestroyFramebuffer(m_device->LogicalDevice, m_frameBuffer, nullptr);
        VX_CORE_INFO("VulkanFrameBuffer: FrameBuffer destroyed.");
    }

    void VulkanFrameBuffer::AddAttachment(vkclass::AttachmentCreateInfo info)
    {
        AddAttachment(info, VK_NULL_HANDLE);
    }

    void VulkanFrameBuffer::AddAttachment(vkclass::AttachmentCreateInfo info, VkImageView view)
    {
        // Create Attachment
        vkclass::FramebufferAttachment attachment;
        attachment.format = info.format;
        
        if(view != VK_NULL_HANDLE)
        {
            attachment.view = view;
        }else
        {
            VX_CORE_INFO("VulkanFrameBuffer: Creating image view for attachment...");
            // create image view
        }
        
        // Fill the attachment description (render pass)
        attachment.description = {};
        attachment.description.format = info.format;
        attachment.description.samples = info.imageSampleCount;
        attachment.description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // this should be depends on usage, but those usage are rare cases
        attachment.description.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // TODO: this should be depends on usage, e.g. intermediate calculations, shadow maps, offscreen rendering
        
        // these two are also rarely used unless for stencil testing
        attachment.description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        
        attachment.description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment.description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // TODO: this should be depends on has depth/stencil or shader, the present image defined would not be here
        
        m_attachments.push_back(attachment);
    }

    void VulkanFrameBuffer::CreateRenderPass()
    {
        std::vector<VkAttachmentDescription> attachmentDescriptions;
        for (auto& attachment : m_attachments)
        {
            attachmentDescriptions.push_back(attachment.description);
        };
        
        // color
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // attachment is intended to function as a color buffer
        
        // TODO: depth / stencil
        
        // default render pass uses only one subpass
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        
        // TODO: use subpass dependencies for attachment layout transition
        
        // create render pass
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.pAttachments = attachmentDescriptions.data();
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.subpassCount = 1;

        VK_CHECK_RESULT(vkCreateRenderPass(m_device->LogicalDevice, &renderPassInfo, nullptr, &m_renderPass));
    }

    void VulkanFrameBuffer::SetUpFrameBuffer()
    {
        std::vector<VkImageView> attachmentViews;
        for (auto attachment : m_attachments)
        {
            attachmentViews.push_back(attachment.view);
        }
        
        // Find max num of layers across attachments
//        uint32_t maxLayers = 0;
//        for (auto attachment : m_attachments)
//        {
//            if (attachment.subresourceRange.layerCount > maxLayers)
//            {
//                maxLayers = attachment.subresourceRange.layerCount;
//            }
//        }
        
        // create framebuffer
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.pAttachments = attachmentViews.data();
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachmentViews.size());
        framebufferInfo.width = Width;
        framebufferInfo.height = Height;
        framebufferInfo.layers = 1;
//        framebufferInfo.layers = maxLayers;
        VK_CHECK_RESULT(vkCreateFramebuffer(m_device->LogicalDevice, &framebufferInfo, nullptr, &m_frameBuffer));
    }
}
