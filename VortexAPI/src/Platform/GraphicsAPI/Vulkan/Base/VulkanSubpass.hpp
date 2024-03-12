#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"

#include "VulkanSwapChain.hpp"
#include "VulkanDevice.hpp"
#include "VulkanTools.hpp"
#include "VulkanInitializer.hpp"

namespace vkclass
{
    struct SubpassAttachment
    {
        VkAttachmentDescription description; // describe format, data operations before/after rendering
        VkAttachmentReference reference; // specify usage, attachment to reference by index for ordering
    };

    // subpass essential variables
    struct SubpassAttachmentInfo
    {
        VkFormat format; // i.e. swapchain format
        
        // enable multi-sampling ?
        VkSampleCountFlagBits imageSampleCount = VK_SAMPLE_COUNT_1_BIT;
    };

    class VulkanSubpass
    {
    public:
        VulkanSubpass(std::string name);
        ~VulkanSubpass();
        
        void AddAttachment(SubpassAttachmentInfo info);
        void Create();
        
        const std::string& Name = m_name;
        const VkSubpassDescription& Subpass = m_subpass;
        const std::vector<SubpassAttachment>& Attachments = m_attachments;
        
    private:
        std::string m_name = "";
        VkSubpassDescription m_subpass{};
        
        std::vector<SubpassAttachment> m_attachments;
        
    };
}
