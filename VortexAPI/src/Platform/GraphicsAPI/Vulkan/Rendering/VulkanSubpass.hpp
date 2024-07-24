#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"

#include "Core/VulkanDevice.hpp"
#include "Core/VulkanTools.hpp"
#include "Core/VulkanInitializer.hpp"
#include "VulkanSwapChain.hpp"

namespace vkclass
{
    enum class SubpassType
    {
        main
    };

    class VulkanSubpass
    {
    public:
        VulkanSubpass(SubpassType subpassType, VkFormat dstFormat);
        ~VulkanSubpass();
        
        const VkSubpassDescription& Subpass = m_subpass;
        //const std::vector<SubpassAttachment>& Attachments = m_attachments;
        
        std::vector<VkAttachmentDescription> GetAttachments() { return m_attachments; }
        std::vector<VkSubpassDependency> GetDependencies() { return m_dependencies; }
    private:
        VkSubpassDescription m_subpass{};
        
        // std::vector<SubpassAttachment> m_attachments;
        std::vector<VkAttachmentDescription> m_attachments;
        std::vector<VkAttachmentReference> m_references;
        std::vector<VkSubpassDependency> m_dependencies;
        
    private:
        // TODO: make it as generic builder functions
        VkAttachmentDescription createAttachment(VkFormat format);
        VkAttachmentReference   createReference();
        VkSubpassDependency     createDependency();
    };
}
