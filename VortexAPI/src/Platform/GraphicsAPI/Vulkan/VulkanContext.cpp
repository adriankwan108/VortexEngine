#include "VulkanContext.hpp"

namespace VX
{
    VulkanContext::VulkanContext(GLFWwindow* window)
    {
        m_glfwExtensions = glfwGetRequiredInstanceExtensions(&m_glfwExtensionCount);
    }

    VulkanContext::~VulkanContext()
    {
        destroyValidationMessenger(true);
        vkDestroyInstance(m_Instance, nullptr);
        VX_CORE_INFO("Vulkan: Instance destroyed.");
    }

    void VulkanContext::Init()
    {
        VX_CORE_INFO("Initiating Vulkan context...");
        createInstance(true);
    }

    void VulkanContext::Display()
    {
        
    }

    VkBool32 VulkanContext::ValidationCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        switch (messageSeverity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            // VX_CORE_TRACE("Vulkan Validation layer: {0}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            // VX_CORE_INFO("Vulkan Validation layer: {0}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            VX_CORE_WARN("Vulkan Validation layer: {0}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            VX_CORE_ERROR("Vulkan Validation layer: {0}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
            VX_CORE_CRITICAL("Vulkan Validation layer: {0}", pCallbackData->pMessage);
            break;
        default:
            break;
        }

        // the callback returns a boolean that indicates if the Vulkan call that triggered the validation layer msg should be aborted
        // if the callback returns true, then the call is aborted with VK_ERROR_VALIDATION_FAILED_EXT error
        // this is normally only used to test the validation layers themselves, so always return VK_FALSE
        return VK_FALSE;
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
        if (enableValidation)
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

                validationMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
                validationMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                validationMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
                validationMessengerCreateInfo.pfnUserCallback = ValidationCallback;
                validationMessengerCreateInfo.pUserData = nullptr; // Optional
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
        }
        else
        {
            VX_CORE_INFO("Vulkan: Instance created.");
        }

        setupValidationMessenger(enableValidation, validationMessengerCreateInfo);
    }

    void VulkanContext::setupValidationMessenger(bool enableValidation, VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        if ( !enableValidation ) { return; }

        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            m_Instance, 
            "vkCreateDebugUtilsMessengerEXT"
        );
        if (func != nullptr)
        {
            if (func(m_Instance, &createInfo, nullptr, &m_validationMessenger) != VK_SUCCESS)
            {
                VX_CORE_ERROR("Vulkan: Failed to set up vaildation messenger.");
                throw std::runtime_error("Vulkan: Failed to set up vaildation messenger.");
            }
        }

        VX_CORE_INFO("Vulkan: Validation messenger is set up");
    }

    void VulkanContext::destroyValidationMessenger(bool enableValidation)
    {
        if (!enableValidation) { return; }

        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            m_Instance, 
            "vkDestroyDebugUtilsMessengerEXT"
        );
        if (func != nullptr)
        {
            func(m_Instance, m_validationMessenger, nullptr);
        }

        VX_CORE_INFO("Vulkan: Validation messenger destroyed");
    }
}
