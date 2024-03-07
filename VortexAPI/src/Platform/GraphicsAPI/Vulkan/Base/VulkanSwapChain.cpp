#include "VulkanSwapChain.hpp"

namespace vkclass
{
    VulkanSwapChain::VulkanSwapChain(vkclass::VulkanSurface* surface, vkclass::VulkanDevice* device): m_surface(surface), m_device(device)
    {
        // query details of swap chain support
        SwapChainSupportDetails swapChainSupportDetails = device->QuerySwapChainSupport(device->PhysicalDevice);
        
        // double verifiy (1st verify at device)
        if(!swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty())
        {
            VX_CORE_INFO("Vulkan SwapChain: Details verified.");
        }else
        {
            VX_CORE_INFO("Vulkan SwapChain: SwapChain is not adequate.");
            std::runtime_error("Vulkan SwapChain: SwapChain is not adequate.");
        }
        
        // create swap chain
        createSwapChain(swapChainSupportDetails);
    }

    VulkanSwapChain::~VulkanSwapChain()
    {
        for (auto imageBuffer : m_swapChainBuffers)
        {
            vkDestroyImageView(m_device->LogicalDevice, imageBuffer.view, nullptr);
        }
        vkDestroySwapchainKHR(m_device->LogicalDevice, m_swapChain, nullptr);
        VX_CORE_INFO("Vulkan SwapChain: Destroy SwapChain");
    }

    void VulkanSwapChain::AcquireNextImage(VkSemaphore semaphore)
    {
        // timeout disabled
        vkAcquireNextImageKHR(m_device->LogicalDevice, m_swapChain, UINT64_MAX, semaphore, VK_NULL_HANDLE, &m_availableImageIndex);
    }

    void VulkanSwapChain::PresentImage(std::vector<VkSemaphore> signalSemaphores)
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
        presentInfo.pWaitSemaphores = signalSemaphores.data();
        
        presentInfo.swapchainCount = 1;
        VkSwapchainKHR swapChains[] = {m_swapChain};
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &m_availableImageIndex;
        presentInfo.pResults = nullptr; // Optional
        
        vkQueuePresentKHR(m_device->PresentQueue, &presentInfo);
    }

    VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        // TODO: implement methods that best suits for out desired needs
        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }
        
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        } else
        {
            // TODO: ask Windows/Mac Window for width & height with glfw
            // instead of calling glfw here
            int width, height;
            glfwGetFramebufferSize(m_surface->Window, &width, &height);

            VkExtent2D actualExtent =
            {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void VulkanSwapChain::createSwapChain(SwapChainSupportDetails details)
    {
        m_surfaceFormat = chooseSwapSurfaceFormat(details.formats);
        m_presentMode = chooseSwapPresentMode(details.presentModes);
        m_extent = chooseSwapExtent(details.capabilities);
        
        m_imageCount = details.capabilities.minImageCount + 1;
        if (details.capabilities.maxImageCount > 0 && m_imageCount > details.capabilities.maxImageCount)
        {
            m_imageCount = details.capabilities.maxImageCount;
        }
        
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface->Surface;
        createInfo.minImageCount = m_imageCount;
        createInfo.imageFormat = m_surfaceFormat.format;
        createInfo.imageColorSpace = m_surfaceFormat.colorSpace;
        createInfo.imageExtent = m_extent;
        createInfo.imageArrayLayers = 1; // this is always 1 unless for a stereoscopic 3D application
        // TODO: Change to VK_IMAGE_USAGE_TRANSFER_DST_BIT for post-processing later
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // specifies what kind of operations we'll use the images in the swap chain for
        
        // specify how to handlw swapchain images that will across different queue families
        QueueFamilyIndices indices = m_device->findQueueFamilies(m_device->PhysicalDevice);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }
        
        createInfo.preTransform = details.capabilities.currentTransform; // specify certain transform to be applied to images in swapchain, e.g. rotate 90 deg clockwise
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;// speicify the alpha channel blending to window
        createInfo.presentMode = m_presentMode;
        createInfo.clipped = VK_TRUE; // don't care about the color of pixels that are obscured, unless we want to get predictable results
        
        createInfo.oldSwapchain = VK_NULL_HANDLE; // for resize event later
        
        VK_CHECK_RESULT(vkCreateSwapchainKHR(m_device->LogicalDevice, &createInfo, nullptr, &m_swapChain));
        VX_CORE_INFO("Vulkan SwapChain: SwapChain created.");
        
        // retrieve the swapchain image
        vkGetSwapchainImagesKHR(m_device->LogicalDevice, m_swapChain, &m_imageCount, nullptr);
        m_swapChainImages.resize(m_imageCount);
        vkGetSwapchainImagesKHR(m_device->LogicalDevice, m_swapChain, &m_imageCount, m_swapChainImages.data());
        
        // Get the swap chain buffers containing the image and imageview
        m_swapChainBuffers.resize(m_imageCount);
        for (uint32_t i = 0; i < m_imageCount; i++)
        {
            VkImageViewCreateInfo colorAttachmentView = {};
            colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            colorAttachmentView.pNext = NULL;
            colorAttachmentView.format = m_surfaceFormat.format;
            colorAttachmentView.components =
            {
                    VK_COMPONENT_SWIZZLE_R,
                    VK_COMPONENT_SWIZZLE_G,
                    VK_COMPONENT_SWIZZLE_B,
                    VK_COMPONENT_SWIZZLE_A
            };
            colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            colorAttachmentView.subresourceRange.baseMipLevel = 0;
            colorAttachmentView.subresourceRange.levelCount = 1;
            colorAttachmentView.subresourceRange.baseArrayLayer = 0;
            colorAttachmentView.subresourceRange.layerCount = 1;
            colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
            colorAttachmentView.flags = 0;

            m_swapChainBuffers[i].image = m_swapChainImages[i];

            colorAttachmentView.image = m_swapChainBuffers[i].image;

            VK_CHECK_RESULT(vkCreateImageView(m_device->LogicalDevice, &colorAttachmentView, nullptr, &m_swapChainBuffers[i].view));
        }
    }
}
