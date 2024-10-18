#pragma once
#include "vulkan/vulkan.h"

namespace vkclass
{
    /*
    RAII Wrapper of VkFramebuffer
    */
    class VulkanFramebuffer
    {
    public:
        VulkanFramebuffer(VkDevice device, VkRenderPass renderPass, const std::vector<VkImageView>& imageViews, const VkExtent2D& extent);
        ~VulkanFramebuffer();

        inline VkFramebuffer GetHandle() const { return m_framebuffer; }
    private:
        VkDevice m_device = VK_NULL_HANDLE;
        VkFramebuffer m_framebuffer = VK_NULL_HANDLE;
    };
}