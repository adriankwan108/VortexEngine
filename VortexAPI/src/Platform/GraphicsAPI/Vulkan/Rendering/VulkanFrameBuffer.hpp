#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"

#include "Core/VulkanDevice.hpp"
#include "Core/VulkanTools.hpp"
#include "Core/VulkanInitializer.hpp"

#include "VulkanSwapChain.hpp"
#include "VulkanRenderPass.hpp"

namespace vkclass
{

    class VulkanFrameBuffer
    {
    public:
        // actually we use swapchain only for extent, this should be cleaned up
        explicit VulkanFrameBuffer(vkclass::VulkanDevice* device, uint32_t width, uint32_t height);
        ~VulkanFrameBuffer();
        
        void AddRenderPass(VX::Ref<vkclass::VulkanRenderPass> renderPass);
        void SetUpFrameBuffer(std::vector<VkImageView> imageViews);
        
    public:

        const VkRenderPass& RenderPass = m_renderPass;
        const VkFramebuffer& FrameBuffer = m_frameBuffer;
        const VkExtent2D& Extent = m_extent; // equal to width and height
        
        
    private:
        // reference
        vkclass::VulkanDevice* m_device;
        
        // props
        VkFramebuffer m_frameBuffer;
        VkRenderPass m_renderPass;
        VkExtent2D m_extent{};
        
    };
}
