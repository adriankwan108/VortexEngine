#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"

#include "VulkanSwapChain.hpp"
#include "VulkanDevice.hpp"
#include "VulkanTools.hpp"
#include "VulkanInitializer.hpp"

namespace vkclass
{
    /**
    * @brief Encapsulates a single frame buffer attachment
    */
    struct FramebufferAttachment
    {
        // images & image view for the frame buffer result
//        VkImage image;
        VkImageView view;
//        VkDeviceMemory memory;
        
        VkFormat format; // for bool func, description (in render pass) format, image view format
        
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
        // info for image views & render pass
        VkFormat format;
        VkSampleCountFlagBits imageSampleCount = VK_SAMPLE_COUNT_1_BIT; // change this if going for multi-sampling
        VkImageUsageFlags usage;
        
        // info for image views only
        uint32_t layerCount;
    };

    class VulkanFrameBuffer
    {
    public:
        // actually we use swapchain only for extent, this should be cleaned up
        explicit VulkanFrameBuffer(vkclass::VulkanDevice* device, vkclass::VulkanSwapChain* swapchain);
        ~VulkanFrameBuffer();
        
        void AddAttachment(vkclass::AttachmentCreateInfo info);
        
        // may be better to have AddAttachment_SwapChain
        void AddAttachment(vkclass::AttachmentCreateInfo info, VkImageView view);
//        void CreateSampler();
//        void SetExtent(uint32_t width, uint32_t height);
        
        void CreateRenderPass();
        void SetUpFrameBuffer();
        
    public:
        // changable extent for offscreen rendering, default set as swapchain extent2D
        uint32_t Width, Height;
        const VkRenderPass& RenderPass = m_renderPass;
        // frame buffer
        // render pass
        // attachments (images)
        //
        
        
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
