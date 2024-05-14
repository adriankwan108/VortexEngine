#include "VulkanDevice.hpp"

namespace vkclass
{
    VulkanDevice::VulkanDevice(vkclass::VulkanInstance* instance, vkclass::VulkanSurface* surface, bool enableValidation): m_enableValidation(enableValidation), m_surface(surface)
    {
        VX_CORE_ASSERT(instance->Instance!=VK_NULL_HANDLE, "Vulkan Device: Null instance");
        
        getPhysicalDevice(instance->Instance);
        createLogicalDevice(instance->RequiredDeviceExtensions);
        VX_CORE_INFO("Vulkan Device created.");
    }

    VulkanDevice::~VulkanDevice()
    {
        vkDestroyDevice(m_logicalDevice, nullptr);
        VX_CORE_INFO("Vulkan Device destroyed.");
    }

    SwapChainSupportDetails VulkanDevice::QuerySwapChainSupport(VkPhysicalDevice device)
    {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface->Surface, &details.capabilities);
        
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface->Surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface->Surface, &formatCount, details.formats.data());
        }
        
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface->Surface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface->Surface, &presentModeCount, details.presentModes.data());
        }
        
        return details;
    }

    void VulkanDevice::GetMemoryInfo(VkBuffer buffer, VkMemoryPropertyFlags targetProps, VkDeviceSize& allocSize, uint32_t& memTypeIndex)
    {
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_logicalDevice, buffer, &memRequirements);
        
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);
        
        allocSize = memRequirements.size;
        memTypeIndex = findMemoryType(memRequirements.memoryTypeBits, memProperties, targetProps);
    }

    void VulkanDevice::GetMemoryInfo(VkImage image, VkMemoryPropertyFlags targetProps, VkDeviceSize& allocSize, uint32_t& memTypeIndex)
    {
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(m_logicalDevice, image, &memRequirements);
        
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);
        
        allocSize = memRequirements.size;
        memTypeIndex = findMemoryType(memRequirements.memoryTypeBits, memProperties, targetProps);
    }

    void VulkanDevice::getPhysicalDevice(VkInstance instance)
    {
        // get number of available GPUs with Vulkan support
        VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &m_gpuCount, nullptr));
        
        if(m_gpuCount == 0)
        {
            VX_CORE_ERROR("Vulkan: No GPUs with Vulkan support is found.");
            throw std::runtime_error("Vulkan: No GPUs with Vulkan support is found.");
            return;
        }
        
        // enumerate GPUs
        m_physicalDevices.resize(m_gpuCount);
        VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &m_gpuCount, m_physicalDevices.data()));
        
        // select gpu
        // Use an ordered map to automatically sort candidates by increasing score
        std::multimap<int, VkPhysicalDevice> candidates;
        for(const auto& gpu: m_physicalDevices)
        {
            if(isGpuSuitable(gpu))
            {
                int score = rateGpuSuitability(gpu);
                candidates.insert(std::make_pair(score, gpu));
                break;
            }
        }
        
        // Check if the best candidate is suitable at all
        if (candidates.rbegin()->first >= 0)
        {
            m_physicalDevice = candidates.rbegin()->second;
        } else
        {
            VX_CORE_ERROR("Vulkan Device: Failed to find a suitable GPU!");
            throw std::runtime_error("VulkanDevice: Failed to find a suitable GPU!");
        }
        
        // store the chosen gpu props, features and limits
        retrieveGpuInfo(m_physicalDevice, m_gpuProps, m_gpuFeatures);
        
        // get list of supported device extensions
        uint32_t extCount = 0;
        vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extCount, nullptr);
        if (extCount > 0)
        {
            std::vector<VkExtensionProperties> extensions(extCount);
            if (vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
            {
                for (auto ext : extensions)
                {
                    m_supportedDeviceExtensions.push_back(ext.extensionName);
                }
            }
        }
        
        VX_CORE_INFO("Vulkan Device: Gpu picking process done.");
        VX_CORE_INFO("Vulkan Device: Using {0} for the application.", m_gpuProps.deviceName);
        
    }

    void VulkanDevice::retrieveGpuInfo(VkPhysicalDevice device, VkPhysicalDeviceProperties& props, VkPhysicalDeviceFeatures& features)
    {
        // query of basic props, e.g. name, type, supported Vulkan version
        vkGetPhysicalDeviceProperties(device, &props);
        
        // query of optional features
        vkGetPhysicalDeviceFeatures(device, &features);
    }

    bool VulkanDevice::isGpuSuitable(VkPhysicalDevice device)
    {
        // query of basic props, e.g. name, type, supported Vulkan version
//        VkPhysicalDeviceProperties deviceProperties;
//        VkPhysicalDeviceFeatures deviceFeatures;
//        retrieveGpuInfo(device, deviceProperties, deviceFeatures);
        
        QueueFamilyIndices indices = findQueueFamilies(device);
        
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
        bool swapChainAdequate = false;
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

        return indices.isComplete() && swapChainAdequate;
    }

    int VulkanDevice::rateGpuSuitability(VkPhysicalDevice device)
    {
        int score = 0;

        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        retrieveGpuInfo(device, deviceProperties, deviceFeatures);
        
        // Discrete GPUs have a significant performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }

        // Maximum possible size of textures affects graphics quality
//        score += deviceProperties.limits.maxImageDimension2D;
//
//        // Application can't function without geometry shaders
//        if (!deviceFeatures.geometryShader)
//        {
//            return 0;
//        }

        return score;
    }

    QueueFamilyIndices VulkanDevice::findQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }
            
            // TODO: try to search for a queue that supports both graphics and present, otherwise, separate
            // Currently don't know the situation
            // TODO: validate the queue index
            // Iterate over each queue to learn whether it supports presenting:
            // Will be used to present the swap chain images to the windowing system
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface->Surface, &presentSupport);
            
            if (presentSupport)
            {
                indices.presentFamily = i;
            }
            
            if(indices.isComplete() && presentSupport)
            {
                break;
            }

            i++;
        }
        return indices;
    }
    
    void VulkanDevice::createLogicalDevice(std::vector<const char *> requiredDeviceExtensions)
    {
        // specifying the queues to be created
        m_QueueFamilyIndices = findQueueFamilies(m_physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies =
        {
            m_QueueFamilyIndices.graphicsFamily.value(),
            m_QueueFamilyIndices.presentFamily.value()
        };
        
        // scheduling of command buffer execution
        float queuePriority = 1.0f; // [0.0, 1.0]
        
        for(uint32_t queueFamily: uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = m_QueueFamilyIndices.graphicsFamily.value();
            queueCreateInfo.queueCount = 1; // no need more than one as we can create all of the command buffers on multiple threads and then submit them all at once on the main thread with a single low-overhead cal
            queueCreateInfo.pQueuePriorities = &queuePriority; // required
            
            queueCreateInfos.emplace_back(queueCreateInfo);
        }
    
        
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        
        VkPhysicalDeviceFeatures enabledGpuFeatures{};
        createInfo.pEnabledFeatures = &enabledGpuFeatures; // nothing
        createInfo.enabledExtensionCount = 0;
        
        // to be compatible with older implementation
        std::vector<const char*> deviceExtensions(requiredDeviceExtensions);
#if defined(__APPLE__)
        // SRS - When running on iOS/macOS with MoltenVK and VK_KHR_portability_subset is defined and supported by the device, enable the extension
        deviceExtensions.push_back("VK_KHR_portability_subset");
#endif
        
        for (const char* extension : deviceExtensions)
        {
            if (!isGpuExtensionSupported(extension))
            {
                VX_CORE_ERROR("Vulkan Device: Extension {0} is not presented at device level", extension);
                throw std::runtime_error("Vulkan Device: Extension is not presented at device level");
            }else
            {
                VX_CORE_INFO("Vulkan Device: Device extension {0} enabled", extension);
            }
        }
        if (deviceExtensions.size() > 0)
        {
            createInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
            createInfo.ppEnabledExtensionNames = deviceExtensions.data();
        }else
        {
            createInfo.enabledExtensionCount = 0;
        }
        
        if(m_enableValidation)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
            createInfo.ppEnabledLayerNames = m_validationLayers.data();
        }else
        {
            createInfo.enabledLayerCount = 0;
        }

        VK_CHECK_RESULT(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice));
        VX_CORE_INFO("Vulkan Device: Logical device created.");
        
        // retrieve queue functions
        vkGetDeviceQueue(m_logicalDevice, m_QueueFamilyIndices.graphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_logicalDevice, m_QueueFamilyIndices.presentFamily.value(), 0, &m_presentQueue);
        m_QueueFamilyIndices.ShowInfo();
    }

    bool VulkanDevice::isGpuExtensionSupported(std::string extension)
    {
        return (std::find(m_supportedDeviceExtensions.begin(), m_supportedDeviceExtensions.end(), extension) != m_supportedDeviceExtensions.end());
    }

    uint32_t VulkanDevice::findMemoryType(uint32_t typeFilter, VkPhysicalDeviceMemoryProperties memProperties, VkMemoryPropertyFlags targetProperties)
    {
        bool validate = false;
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & targetProperties) == targetProperties)
            {
                validate = true;
                return i;
            }
        }
        if(validate == false)
        {
            VX_CORE_ERROR("Vulkan Device: Failed to find suitable memory type!");
            throw std::runtime_error("Vulkan Device: Failed to find suitable memory type!");
        }
    }
}
