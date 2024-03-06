#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"
#include "VulkanDevice.hpp"

#include "VulkanSurface.hpp"
#include <GLFW/glfw3.h>

namespace vkclass
{
    typedef struct _SwapChainBuffers {
        VkImage image;
        VkImageView view;
    } SwapChainBuffer;

    class VulkanSwapChain
    {
    public:
        explicit VulkanSwapChain(vkclass::VulkanSurface* surface, vkclass::VulkanDevice* device);
        ~VulkanSwapChain();
        
        const uint32_t& ImageCount = m_imageCount; // equal to SwapChainBuffers size
        const VkSurfaceFormatKHR& SurfaceFormat = m_surfaceFormat;
        const VkExtent2D& Extent = m_extent;
        const std::vector<SwapChainBuffer>& ImageBuffers = m_swapChainBuffers;
        
    private:
        vkclass::VulkanSurface* m_surface;
        vkclass::VulkanDevice* m_device;
        
        VkSurfaceFormatKHR m_surfaceFormat;
        VkPresentModeKHR m_presentMode;
        VkExtent2D m_extent;
        
        uint32_t m_imageCount;
        
        VkSwapchainKHR m_swapChain;
        std::vector<VkImage> m_swapChainImages; // TODO: this should be cleaned up
        std::vector<SwapChainBuffer> m_swapChainBuffers;
        
    private:
        // @brief choose color space
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        
        // @brief resolution of swapchain images
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        
        void createSwapChain(SwapChainSupportDetails details);
    };
}
