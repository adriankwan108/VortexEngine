#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>

#include "Core/VulkanDevice.hpp"
#include "Core/VulkanSurface.hpp"

namespace vkclass
{
    typedef struct _SwapChainBuffers {
        VkImage image;
        VkImageView view;
    } SwapChainBuffer;

    class VulkanSwapChain
    {
    public:
        VulkanSwapChain(vkclass::VulkanSurface* surface);
        ~VulkanSwapChain();
        
        static void Init(vkclass::VulkanDevice* device);
        
        VkResult AcquireNextImage(VkSemaphore semaphore);
        VkResult PresentImage(std::vector<VkSemaphore> signalSemaphores);
        void CreateSwapChain(bool isVSync = false);
        
    public:
        static vkclass::VulkanDevice* m_device;
        
        const uint32_t& ImageCount = m_imageCount;
        const uint32_t& AvailableImageIndex = m_availableImageIndex;
        
        const VkSurfaceFormatKHR& SurfaceFormat = m_surfaceFormat;
        const VkExtent2D& Extent = m_extent;
        const std::vector<SwapChainBuffer>& ImageBuffers = m_swapChainBuffers;
        
    private:
        vkclass::VulkanSurface* m_surface;
        
        VkSurfaceFormatKHR m_surfaceFormat{};
        VkPresentModeKHR m_presentMode{};
        VkExtent2D m_extent{};
        
        uint32_t m_imageCount; // equal to SwapChainBuffers size
        uint32_t m_availableImageIndex; // index of the swap chain image that has become available.
        
        VkSwapchainKHR m_swapChain;
        std::vector<VkImage> m_swapChainImages; // TODO: this should be cleaned up
        std::vector<SwapChainBuffer> m_swapChainBuffers;
        
    private:
        // @brief choose color space
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes, bool isVSync);
        
        // @brief resolution of swapchain images
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        
    };
}
