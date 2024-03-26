#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"

#include "Core/VulkanDevice.hpp"
#include "Core/VulkanTools.hpp"
#include "Core/VulkanInitializer.hpp"
#include "VulkanSwapChain.hpp"

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
