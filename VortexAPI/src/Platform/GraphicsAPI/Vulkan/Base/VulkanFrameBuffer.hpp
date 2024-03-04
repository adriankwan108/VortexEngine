#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"

#include "VulkanSwapChain.hpp"
#include "VulkanDevice.hpp"
#include "VulkanTools.hpp"

namespace vkclass
{
    /**
    * @brief Encapsulates a single frame buffer attachment
    */
    struct FramebufferAttachment
    {
        VkImage image;
        VkImageView view;
        VkDeviceMemory memory;
        
        VkFormat format;
//        VkImageSubresourceRange subresourceRange;
        VkAttachmentDescription description;

        /**
        * @brief Returns true if the attachment has a depth component
        */
        bool hasDepth()
        {
            std::vector<VkFormat> formats =
            {
                VK_FORMAT_D16_UNORM,
                VK_FORMAT_X8_D24_UNORM_PACK32,
                VK_FORMAT_D32_SFLOAT,
                VK_FORMAT_D16_UNORM_S8_UINT,
                VK_FORMAT_D24_UNORM_S8_UINT,
                VK_FORMAT_D32_SFLOAT_S8_UINT,
            };
            return std::find(formats.begin(), formats.end(), format) != std::end(formats);
        }

        /**
        * @brief Returns true if the attachment has a stencil component
        */
        bool hasStencil()
        {
            std::vector<VkFormat> formats =
            {
                VK_FORMAT_S8_UINT,
                VK_FORMAT_D16_UNORM_S8_UINT,
                VK_FORMAT_D24_UNORM_S8_UINT,
                VK_FORMAT_D32_SFLOAT_S8_UINT,
            };
            return std::find(formats.begin(), formats.end(), format) != std::end(formats);
        }

        /**
        * @brief Returns true if the attachment is a depth and/or stencil attachment
        */
        bool isDepthStencil()
        {
            return(hasDepth() || hasStencil());
        }

    };
    /**
    * @brief Describes the attributes of an attachment to be created
    */
    struct AttachmentCreateInfo
    {
//        uint32_t width, height;
//        uint32_t layerCount;
        VkFormat format;
//        VkImageUsageFlags usage;
        VkSampleCountFlagBits imageSampleCount = VK_SAMPLE_COUNT_1_BIT; // if not doing anything with multi-sampling
    };

    class VulkanFrameBuffer
    {
    public:
        explicit VulkanFrameBuffer(vkclass::VulkanDevice* device, vkclass::VulkanSwapChain* swapchain);
        ~VulkanFrameBuffer();
        
        void AddAttachment(vkclass::AttachmentCreateInfo info);
        void CreateSampler();
        void CreateRenderPass();
        void SetUpFrameBuffer();
        
    public:
        // changable extent for offscreen rendering, default set as swapchain extent2D
        uint32_t Width, Height;
        
    private:
        // reference
        vkclass::VulkanDevice* m_device;
        vkclass::VulkanSwapChain* m_swapChain;
        
        // props
        VkFramebuffer m_frameBuffer;
        std::vector<FramebufferAttachment> m_attachments;
        VkRenderPass m_renderPass;
    };
}
