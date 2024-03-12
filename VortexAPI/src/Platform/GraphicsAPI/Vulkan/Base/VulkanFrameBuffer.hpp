#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"

#include "VulkanSwapChain.hpp"
#include "VulkanDevice.hpp"
#include "VulkanTools.hpp"
#include "VulkanInitializer.hpp"
#include "VulkanRenderPass.hpp"

namespace vkclass
{

    class VulkanFrameBuffer
    {
    public:
        // actually we use swapchain only for extent, this should be cleaned up
        explicit VulkanFrameBuffer(vkclass::VulkanDevice* device, vkclass::VulkanSwapChain* swapchain);
        ~VulkanFrameBuffer();
        
        void AddRenderPass(vkclass::VulkanRenderPass* renderPass);
        void SetUpFrameBuffer(std::vector<VkImageView> imageViews);
        
    public:
        // changable extent for offscreen rendering, default set as swapchain extent2D
        uint32_t Width, Height;
        const VkRenderPass& RenderPass = m_renderPass;
        const VkFramebuffer& FrameBuffer = m_frameBuffer;
        const VkExtent2D& Extent = m_extent; // equal to width and height
        
        
    private:
        // reference
        vkclass::VulkanDevice* m_device;
        vkclass::VulkanSwapChain* m_swapChain;
        
        // props
        VkFramebuffer m_frameBuffer;
        VkRenderPass m_renderPass;
        VkExtent2D m_extent{};
        
    };
}
