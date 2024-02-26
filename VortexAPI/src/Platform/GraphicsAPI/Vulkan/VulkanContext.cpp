#include "VulkanContext.hpp"

namespace VX
{
    VulkanContext::VulkanContext(GLFWwindow* window)
    {
        m_glfwExtensions = glfwGetRequiredInstanceExtensions(&m_glfwExtensionCount);
    }

    VulkanContext::~VulkanContext()
    {
        if(m_enableValidation)
        {
            vkclass::debug::clearValidation(m_Instance);
        }
        vkDestroyInstance(m_Instance, nullptr);
        VX_CORE_INFO("Vulkan: Instance destroyed.");
    }

    void VulkanContext::Init()
    {
        VX_CORE_INFO("Initiating Vulkan context...");
        createInstance(true);
        pickDevice();
    }

    void VulkanContext::Display()
    {
        
    }

    void VulkanContext::createInstance(bool enableValidation)
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.apiVersion = VK_API_VERSION_1_0;
        
        appInfo.pApplicationName = "temp app"; // TODO: should be Window Data
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: follow cmake
        
        appInfo.pEngineName = "temp engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: follow cmake
        
        // Get extensions supported by the instance
        uint32_t extCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extCount);
        if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
        {
            for (VkExtensionProperties& extension : extensions)
            {
                m_supportedInstanceExtensions.emplace_back(extension.extensionName);
            }
        }
         
        // get all required extensions
#if (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
        // SRS - When running on iOS/macOS with MoltenVK, enable VK_KHR_get_physical_device_properties2 if not already enabled (required by VK_KHR_portability_subset)
        if (std::find(m_requiredInstanceExtensions.begin(), m_requiredInstanceExtensions.end(), VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) == enabledInstanceExtensions.end())
        {
            m_requiredInstanceExtensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        }
#endif
      
        for(uint32_t i = 0; i < m_glfwExtensionCount; i++) {
            m_requiredInstanceExtensions.emplace_back(m_glfwExtensions[i]);
        }

        if (enableValidation)
        {
            m_requiredInstanceExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        
        // Enabled requested instance extensions
        if (m_requiredInstanceExtensions.size() > 0)
        {
            for (const char * requiredExtension : m_requiredInstanceExtensions)
            {
                // Output message if requested extension is not available
                if (std::find(m_supportedInstanceExtensions.begin(), m_supportedInstanceExtensions.end(), requiredExtension) == m_supportedInstanceExtensions.end())
                {
                    VX_CORE_ERROR("Vulkan: Required instance extensions {0} is not supported at instance level.", requiredExtension);
                }else
                {
                    VX_CORE_INFO("Vulkan: Instance extension: {0} enabled", requiredExtension);
                    m_enabledInstanceExtensions.emplace_back(requiredExtension);
                }
            }
        }
        
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//      createInfo.pNext = NULL;
        createInfo.pApplicationInfo = &appInfo;
         
#if defined(__APPLE__)
//      SRS - When running on iOS/macOS with MoltenVK and VK_KHR_portability_enumeration is defined and supported by the instance, enable the extension and the flag
        if (std::find(m_supportedInstanceExtensions.begin(), m_supportedInstanceExtensions.end(), VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) != m_supportedInstanceExtensions.end())
        {
            m_enabledInstanceExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        }
#endif
        
        createInfo.enabledExtensionCount = (uint32_t)m_enabledInstanceExtensions.size();
        createInfo.ppEnabledExtensionNames = m_enabledInstanceExtensions.data();
        createInfo.enabledLayerCount = 0;

        // The VK_LAYER_KHRONOS_validation contains all current validation functionality.
        // Note that on Android this layer requires at least NDK r20
        const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
        VkDebugUtilsMessengerCreateInfoEXT validationMessengerCreateInfo{};
        if (m_enableValidation)
        {
            // Check if this layer is available at instance level
            uint32_t instanceLayerCount;
            vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
            std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
            vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
            bool validationLayerPresent = false;
            for (const VkLayerProperties& layer : instanceLayerProperties) 
            {
                if (strcmp(layer.layerName, validationLayerName) == 0) 
                {
                    validationLayerPresent = true;
                    break;
                }
            }
            if (validationLayerPresent) 
            {
                createInfo.ppEnabledLayerNames = &validationLayerName;
                createInfo.enabledLayerCount = 1;
                VX_CORE_INFO("Vulkan: Validation layer enabled");

                vkclass::debug::setupValidationMessengerCI(validationMessengerCreateInfo);
                createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&validationMessengerCreateInfo;
            }
            else 
            {
                createInfo.enabledLayerCount = 0;
                VX_CORE_WARN("Vulkan: Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled");
            }
        }

        if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
        {
            VX_CORE_ERROR("Vulkan: Failed to create instance!");
            throw std::runtime_error("Vulkan: Failed to create instance!");
            return;
        }
        else
        {
            VX_CORE_INFO("Vulkan: Instance created.");
        }

        if(m_enableValidation)
        {
            VkDebugUtilsMessengerCreateInfoEXT validationCreateInfo;
            vkclass::debug::setupValidation(m_Instance);
        }
    }

    void VulkanContext::pickDevice()
    {
        // get number of available GPUs with Vulkan support
        uint32_t gpuCount = 0;
        VK_CHECK_RESULT(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, nullptr));
        
        if(gpuCount == 0)
        {
            VX_CORE_ERROR("Vulkan: No GPUs with Vulkan support is found.");
            throw std::runtime_error("Vulkan: No GPUs with Vulkan support is found.");
            return;
        }
        
        // enumerate GPUs
        std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
        VK_CHECK_RESULT(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, physicalDevices.data()));
        
        // select GPUs
        // Currently, just find the first suitable GPU, as I have only one GPU in hand.
        // TODO: Score all device and pick the highest one
        // TODO: Enable certain features
        for(const auto& gpu: physicalDevices)
        {
            if(vkclass::VulkanDevice::IsDeviceSuitable(gpu))
            {
                m_gpu = gpu;
                break;
            }
        }
//        m_gpu = physicalDevices[0];
        
        if(m_gpu == VK_NULL_HANDLE)
        {
            VX_CORE_ERROR("Vulkan: Failed to find a suitable GPU.");
            throw std::runtime_error("Vulkan: Failed to find a suitable GPU.");
        }
        
        m_VulkanDevice = new vkclass::VulkanDevice(m_gpu);
     
    }
}
