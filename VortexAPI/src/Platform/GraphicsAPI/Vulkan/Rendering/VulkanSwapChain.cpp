#include "VulkanSwapChain.hpp"

namespace vkclass
{
    VulkanSwapChain::VulkanSwapChain(vkclass::VulkanDevice* device, vkclass::VulkanSurface* surface) : m_device(device), m_surface(surface)
    {
        
    }

    VulkanSwapChain::~VulkanSwapChain()
    {
        for (auto& imageBuffer : m_swapChainBuffers)
        {
            vkDestroyImageView(m_device->LogicalDevice, imageBuffer.view, nullptr);
        }
        vkDestroySwapchainKHR(m_device->LogicalDevice, m_swapChain, nullptr);
        m_swapChain = VK_NULL_HANDLE;
        VX_CORE_INFO("VulkanSwapChain: Destroy SwapChain");
    }

    VkResult VulkanSwapChain::AcquireNextImage(VkSemaphore semaphore)
    {
        // timeout disabled
        return vkAcquireNextImageKHR(m_device->LogicalDevice, m_swapChain, UINT64_MAX, semaphore, VK_NULL_HANDLE, &m_availableImageIndex);
    }

    VkResult VulkanSwapChain::PresentImage(std::vector<VkSemaphore> signalSemaphores)
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
        
        return vkQueuePresentKHR(m_device->PresentQueue, &presentInfo);
    }

    void VulkanSwapChain::CreateSwapChain(bool isVSync, bool isFullScreen)
    {
        // Store the current swap chain handle so we can use it later on to ease up recreation
        VkSwapchainKHR oldSwapchain = m_swapChain;

        // query details of swap chain support
        SwapChainSupportDetails details = m_device->QuerySwapChainSupport(m_device->PhysicalDevice);
        
        // double verifiy (1st verify at device)
        if(!details.formats.empty() && !details.presentModes.empty())
        {
            VX_CORE_INFO("VulkanSwapChain::Create: Details verified.");
        }else
        {
            VX_CORE_ERROR("Vulkan SwapChain: SwapChain is not supported.");
            throw std::runtime_error("VulkanSwapChain::Create: SwapChain is not supported.");
        }
        
        m_surfaceFormat = chooseSwapSurfaceFormat(details.formats);
        m_presentMode = chooseSwapPresentMode(details.presentModes, isVSync);
        m_extent = chooseSwapExtent(details.capabilities);
        
        uint32_t desiredNumberOfSwapchainImages = details.capabilities.minImageCount + 1;
        #if (defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT)) && defined(VK_EXAMPLE_XCODE_GENERATED)
            // Work around known MoltenVK issue re 2x frame rate when vsync (VK_PRESENT_MODE_FIFO_KHR) enabled
            struct utsname sysInfo;
            uname(&sysInfo);
            // SRS - When vsync is on, use minImageCount when not in fullscreen or when running on Apple Silcon
            // This forces swapchain image acquire frame rate to match display vsync frame rate
            if (isVSync && (!isFullScreen || strcmp(sysInfo.machine, "arm64") == 0))
            {
                desiredNumberOfSwapchainImages = details.capabilities.minImageCount;
            }
        #endif
        if (details.capabilities.maxImageCount > 0 && desiredNumberOfSwapchainImages > details.capabilities.maxImageCount)
        {
            desiredNumberOfSwapchainImages = details.capabilities.maxImageCount;
        }
        
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface->Surface;
        createInfo.minImageCount = desiredNumberOfSwapchainImages;
        createInfo.imageFormat = m_surfaceFormat.format;
        createInfo.imageColorSpace = m_surfaceFormat.colorSpace;
        createInfo.imageExtent = m_extent;
        createInfo.imageArrayLayers = 1; // this is always 1 unless for a stereoscopic 3D application

        // specifies what kind of operations the images used in the swap chain for
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        // Enable transfer src / dst on swap chain images if supported
        if (details.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
        {
            createInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            VX_CORE_INFO("VulkanSwapChain::Create: ImageUsage: Transfer_src enabled");
        }
        if (details.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
        {
            createInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            VX_CORE_INFO("VulkanSwapChain::Create: ImageUsage: Transfer_dst enabled");
        }
        
        // specify how to handlw swapchain images that will across different queue families
        QueueFamilyIndices indices = m_device->findQueueFamilies(m_device->PhysicalDevice);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
            VX_CORE_INFO("VulkanSwapChain::Create: ImageSharingMode: Concurrent");
        } else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
            VX_CORE_INFO("VulkanSwapChain::Create: ImageSharingMode: Exclusive");
        }
        
        // specify certain transform to be applied to images in swapchain, e.g. rotate 90 deg clockwise
        if (details.capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        {
            // prefer a non-rotated transform
            createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        }
        else
        {
            createInfo.preTransform = details.capabilities.currentTransform;
        }
        VX_CORE_INFO("VulkanSwapChain::Create: PreTransform: {0}", (int)createInfo.preTransform);

        // find and speicify the supported composte alpha channel blending to window (as not all device support VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR )
        VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
            VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
        };
        for (auto& compositeAlphaFlag : compositeAlphaFlags) 
        {
            if (details.capabilities.supportedCompositeAlpha & compositeAlphaFlag) 
            {
                compositeAlpha = compositeAlphaFlag;
                break;
            };
        }
        createInfo.compositeAlpha = compositeAlpha;
        VX_CORE_INFO("VulkanSwapChain::Create: CompositeAlpha: {0}", (int)createInfo.compositeAlpha);

        createInfo.presentMode = m_presentMode;
        // Setting clipped to VK_TRUE allows the implementation to discard rendering outside of the surface area, unless desired for predictable results
        createInfo.clipped = VK_TRUE;
        
        createInfo.oldSwapchain = oldSwapchain;
        
        VK_CHECK_RESULT(vkCreateSwapchainKHR(m_device->LogicalDevice, &createInfo, nullptr, &m_swapChain));
        VX_CORE_INFO("VulkanSwapChain: SwapChain created.");

        // If an existing swap chain is re-created, destroy the old swap chain and cleans up all the presentable images
        if (oldSwapchain != VK_NULL_HANDLE)
        {
            for (auto& imageBuffer : m_swapChainBuffers)
            {
                vkDestroyImageView(m_device->LogicalDevice, imageBuffer.view, nullptr);
            }
            vkDestroySwapchainKHR(m_device->LogicalDevice, oldSwapchain, nullptr);
        }
        VK_CHECK_RESULT(vkGetSwapchainImagesKHR(m_device->LogicalDevice, m_swapChain, &m_imageCount, NULL));
        
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

    VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes, bool isVSync)
    {
        // Note: VK_PRESENT_MODE_FIFO_KHR is guaranteed
        if (!isVSync)
        {
            // If v-sync is not requested, try to find a mailbox mode, it's the lowest latency non-tearing present mode available
            for (const auto& availablePresentMode : availablePresentModes)
            {
                if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                {
                    VX_CORE_INFO("VulkanSwapChain::ChoosePresentMode: {0}", "VK_PRESENT_MODE_MAILBOX_KHR");
                    return availablePresentMode;
                }
                if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
                {
                    VX_CORE_INFO("VulkanSwapChain::ChoosePresentMode: {0}", "VK_PRESENT_MODE_IMMEDIATE_KHR");
                    return availablePresentMode;
                }
            }
        }
        else
        {
            VX_CORE_INFO("VulkanSwapChain::ChoosePresentMode: {0}", "VK_PRESENT_MODE_FIFO_KHR");
            return VK_PRESENT_MODE_FIFO_KHR;
        }
    }

    VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        } else
        {
            VkExtent2D actualExtent{};
            auto surfaceWidth = m_surface->GetExtent().width;
            auto surfaceHeight = m_surface->GetExtent().height;

            actualExtent.width = std::clamp(surfaceWidth, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(surfaceHeight, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }
}
