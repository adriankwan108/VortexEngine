#include "VulkanSubpass.hpp"

namespace vkclass
{
    VulkanSubpass::VulkanSubpass(SubpassType subpassType, VkFormat dstFormat)
    {
        switch (subpassType) {
            case SubpassType::main:
                m_attachments.push_back(createAttachment(dstFormat));
                m_references.push_back(createReference());
                m_dependencies.push_back(createDependency());
                
                m_subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                m_subpass.colorAttachmentCount = static_cast<uint32_t>( m_references.size());
                m_subpass.pColorAttachments = m_references.data();
 
            break;
                
            default:
            break;
        }
    }

    VulkanSubpass::~VulkanSubpass()
    {
        
    }

    VkAttachmentDescription VulkanSubpass::createAttachment(VkFormat format)
    {
        // description
        VkAttachmentDescription description{};
        description.format = format;
        description.samples = VK_SAMPLE_COUNT_1_BIT;
        
        // TODO: modify this depends on usage, this setting is geometry for now
        description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        
        return description;
    }

    VkAttachmentReference   VulkanSubpass::createReference()
    {
        VkAttachmentReference reference{};
        reference.attachment = 0;
        reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // intended as color buffer
        
        return reference;
    }

    VkSubpassDependency VulkanSubpass::createDependency()
    {
        // TODO: make it generic, now is just for main
        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL; // implicit subpass before or after the render pass depending on whether it is specified in srcSubpass or dstSubpass
        dependency.dstSubpass = 0; // refers to the first subpass
        
        // wait on operation stages
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // wait for swapchain finish reading image
        dependency.srcAccessMask = 0;
        
        // prevent the transition from happening until it's actually necessary
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        return dependency;
    }
}
