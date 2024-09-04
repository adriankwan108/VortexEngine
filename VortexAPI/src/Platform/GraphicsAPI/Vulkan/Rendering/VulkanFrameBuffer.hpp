#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"

#include "Vortex/Renderer/FrameBuffer.hpp"
#include "Core/VulkanDevice.hpp"
#include "Core/VulkanTools.hpp"
#include "Core/VulkanInitializer.hpp"

#include "VulkanSwapChain.hpp"
#include "VulkanRenderPass.hpp"



namespace vkclass
{
    // Manages all dynamically created VulkanFrameBuffer
    class VulkanFrameBuffersManager
    {
    public:
        void Resize();
    private:
        
    };

    class VulkanFrameBuffer : public VX::FrameBuffer
    {
    public:
        VulkanFrameBuffer(const VX::FramebufferSpecification& spec);
        ~VulkanFrameBuffer() = default;
        
        virtual void Bind();
        virtual void UnBind();
        
        virtual void Resize(uint32_t width, uint32_t height);
        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y);
        
        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const;
        virtual const VX::FramebufferSpecification& GetSpecification() const;
        
        virtual void ClearAttachment(uint32_t attachmentIndex, int value);
    
    private:
        VX::FramebufferSpecification m_spec;
        
        // vkextent
        // vkrenderpass
        // vkframebuffer
    };

    

//    // Abstracted FrameBuffer composed of vkframebuffer and vkrenderpass
//    class VulkanFrameBuffer
//    {
//    public:
//        // actually we use swapchain only for extent, this should be cleaned up
//        explicit VulkanFrameBuffer(vkclass::VulkanDevice* device, uint32_t width, uint32_t height);
//        ~VulkanFrameBuffer();
//
//        void SetRenderPass(const VkRenderPass& renderPass);
//        void SetUpFrameBuffer(std::vector<VkImageView> imageViews);
//
//    public:
//
//        const VkRenderPass& RenderPass = m_renderPass;
//        const VkFramebuffer& FrameBuffer = m_frameBuffer;
//        const VkExtent2D& Extent = m_extent; // equal to width and height
//
//
//    private:
//        // reference
//        vkclass::VulkanDevice* m_device;
//
//        // props
//        VkFramebuffer m_frameBuffer;
//        VkRenderPass m_renderPass;
//        VkExtent2D m_extent{};
//
//    };
}
