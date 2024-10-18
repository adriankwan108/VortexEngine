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

    class VulkanRenderTargetManager;

    class VulkanSwapChain
    {
    public:
        VulkanSwapChain(vkclass::VulkanDevice* device, vkclass::VulkanSurface* surface);
        ~VulkanSwapChain();
        
        VkResult AcquireNextImage(VkSemaphore semaphore);
        VkResult PresentImage(std::vector<VkSemaphore> signalSemaphores);
        void CreateSwapChain(bool isVSync, bool isFullScreen);
        
        inline uint32_t                     GetImageCount()     const { return m_imageCount; }
        inline VkSurfaceFormatKHR           GetSurfaceFormat()  const { return m_surfaceFormat; }
        inline VkImage                      GetCurrentImage()   const { return m_swapChainImages[m_availableImageIndex]; }
        inline VkExtent2D                   GetExtent()         const { return m_extent; }
        inline std::vector<SwapChainBuffer> GetImageBuffers()   const { return m_swapChainBuffers; }
        
    private:
        friend class VulkanRenderTargetManager;

        vkclass::VulkanDevice* m_device;
        vkclass::VulkanSurface* m_surface;
        
        VkSurfaceFormatKHR m_surfaceFormat{};
        VkPresentModeKHR m_presentMode{};
        VkExtent2D m_extent{};
        
        uint32_t m_imageCount = 0; 
        uint32_t m_availableImageIndex = 0; // index of the swap chain image that has become available.
        
        VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
        std::vector<VkImage> m_swapChainImages;
        std::vector<SwapChainBuffer> m_swapChainBuffers; // size = m_imageCount (to be determined by platform)
        
 
        // @brief choose color space
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes, bool isVSync);
        
        // @brief resolution of swapchain images
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        
    };
}
