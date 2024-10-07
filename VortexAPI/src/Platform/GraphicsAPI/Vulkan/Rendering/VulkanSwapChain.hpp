#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>

#include "Core/VulkanDevice.hpp"
#include "Core/VulkanSurface.hpp"

#ifdef __APPLE__
#include <sys/utsname.h>
#endif

namespace vkclass
{
    typedef struct _SwapChainBuffers {
        VkImage image;
        VkImageView view;
    } SwapChainBuffer;

    class VulkanSwapChain
    {
    public:
        VulkanSwapChain(vkclass::VulkanDevice* device, vkclass::VulkanSurface* surface);
        ~VulkanSwapChain();
        
        VkResult AcquireNextImage(VkSemaphore semaphore);
        VkResult PresentImage(std::vector<VkSemaphore> signalSemaphores);
        void CreateSwapChain(bool isVSync, bool isFullScreen);
        
    public:
        vkclass::VulkanDevice* m_device;
        
        const uint32_t& ImageCount = m_imageCount;
        const uint32_t& AvailableImageIndex = m_availableImageIndex;
        
        inline VkSurfaceFormatKHR GetSurfaceFormat() const { return m_surfaceFormat; }
        const VkExtent2D& Extent = m_extent;
        const std::vector<SwapChainBuffer>& ImageBuffers = m_swapChainBuffers;
        
        VkImage GetCurrentImage() { return m_swapChainImages[m_availableImageIndex]; }
        
    private:
        vkclass::VulkanSurface* m_surface;
        
        VkSurfaceFormatKHR m_surfaceFormat{};
        VkPresentModeKHR m_presentMode{};
        VkExtent2D m_extent{};
        
        uint32_t m_imageCount; 
        uint32_t m_availableImageIndex; // index of the swap chain image that has become available.
        
        VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
        std::vector<VkImage> m_swapChainImages;
        std::vector<SwapChainBuffer> m_swapChainBuffers; // size = m_imageCount (to be determined by platform)
        
    private:
        // @brief choose color space
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes, bool isVSync);
        
        // @brief resolution of swapchain images
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        
    };
}
