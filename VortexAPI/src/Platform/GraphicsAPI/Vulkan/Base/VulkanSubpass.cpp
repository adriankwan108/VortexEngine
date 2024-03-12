#include "VulkanSubpass.hpp"

namespace vkclass
{
    VulkanSubpass::VulkanSubpass(std::string name):m_name(name)
    {
        
    }

    VulkanSubpass::~VulkanSubpass()
    {
        
    }

    void VulkanSubpass::AddAttachment(SubpassAttachmentInfo info)
    {
        // create the attachment
        SubpassAttachment attachment;
        
        // description
        VkAttachmentDescription description{};
        description.format = info.format;
        description.samples = info.imageSampleCount;
        
        // TODO: modify this depends on usage, this setting is geometry for now
        description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        
        // reference
        // TODO: modify this depends on usage, and attachment counter
        VkAttachmentReference reference{};
        reference.attachment = 0;
        reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // intended as color buffer
        
        // push to attachment list
        attachment.description = description;
        attachment.reference = reference;
        m_attachments.push_back(attachment);
    }

    void VulkanSubpass::Create()
    {
        m_subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        
        // TODO: modifty this based on attachments, which should conquer the attachments
        m_subpass.pColorAttachments = &m_attachments[0].reference;
        m_subpass.colorAttachmentCount = 1;
    }
}
