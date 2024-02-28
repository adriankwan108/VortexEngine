#include "VulkanInstance.hpp"

namespace vkclass
{
    VulkanInstance::VulkanInstance(VkApplicationInfo appInfo, bool enableValidation):m_enableValidation(enableValidation)
    {
        getSupportedExtensions();
        getRequiredExtensions();
        enableExtensions();
        createInstance(appInfo);
    }

    VulkanInstance::~VulkanInstance()
    {
        vkDestroySurfaceKHR(m_Instance, m_surface, nullptr);
        VX_CORE_INFO("Vulkan Instance: Surface destroyed");
        if(m_enableValidation)
        {
            vkclass::debug::clearValidation(m_Instance);
        }
        vkDestroyInstance(m_Instance, nullptr);
        VX_CORE_INFO("Vulkan: Instance destroyed.");
    }

    void VulkanInstance::CreateSurface(GLFWwindow* window)
    {
        // create surface, which should be platform agnostic, but glfw already takes care
        if (glfwCreateWindowSurface(m_Instance, window, nullptr, &m_surface) != VK_SUCCESS)
        {
               throw std::runtime_error("failed to create window surface!");
        }else
        {
            VX_CORE_INFO("Vulkan Instance: Surface created.");
        }
        
    }

    void VulkanInstance::getSupportedExtensions()
    {
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
    }

    void VulkanInstance::getRequiredExtensions()
    {

// #if (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
#if defined(__APPLE__)
        // SRS - When running on iOS/macOS with MoltenVK, enable VK_KHR_get_physical_device_properties2 if not already enabled (required by VK_KHR_portability_subset)
            m_requiredInstanceExtensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        
        // SRS - When running on iOS/macOS with MoltenVK and VK_KHR_portability_enumeration is defined and supported by the instance, enable the extension and the flag
        if (std::find(m_supportedInstanceExtensions.begin(), m_supportedInstanceExtensions.end(), VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) != m_supportedInstanceExtensions.end())
        {
            m_requiredInstanceExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        }else
        {
            VX_CORE_WARN("VulkanInstance: Not supporting iOS/macOS portability enumeration");
        }
#endif

        m_glfwExtensions = glfwGetRequiredInstanceExtensions(&m_glfwExtensionCount);
        for(uint32_t i = 0; i < m_glfwExtensionCount; i++) {
            m_requiredInstanceExtensions.emplace_back(m_glfwExtensions[i]);
        }

        if (m_enableValidation)
        {
            m_requiredInstanceExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
    }

    void VulkanInstance::enableExtensions()
    {
        bool check = true;
        if (m_requiredInstanceExtensions.size() > 0)
        {
            for (const char * requiredExtension : m_requiredInstanceExtensions)
            {
                // Output message if requested extension is not available
                if (std::find(m_supportedInstanceExtensions.begin(), m_supportedInstanceExtensions.end(), requiredExtension) == m_supportedInstanceExtensions.end())
                {
                    VX_CORE_ERROR("VulkanInstance: Required extension: {0} is not supported at instance level.", requiredExtension);
                     check = false;
                }else
                {
                    VX_CORE_INFO("VulkanInstance: Extension: {0} enabled", requiredExtension);
                    m_enabledInstanceExtensions.emplace_back(requiredExtension);
                }
            }
        }

        if(check == false)
        {
            VX_CORE_ERROR("VulkanInstance: creating process stops.");
            throw std::runtime_error("VulkanInstance: creating process stops.");
        }
    }

    void VulkanInstance::createInstance(VkApplicationInfo appInfo)
    {
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

#if defined(__APPLE__)
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
        createInfo.enabledExtensionCount = (uint32_t)m_enabledInstanceExtensions.size();
        createInfo.ppEnabledExtensionNames = m_enabledInstanceExtensions.data();
        createInfo.enabledLayerCount = 0;

        if(m_enableValidation)
        {
            // The VK_LAYER_KHRONOS_validation contains all current validation functionality.
            // Note that on Android this layer requires at least NDK r20
            const char* validationLayerName = "VK_LAYER_KHRONOS_validation";

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

                VkDebugUtilsMessengerCreateInfoEXT validationMessengerCreateInfo{};
                vkclass::debug::setupValidationMessengerCI(validationMessengerCreateInfo);
                createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&validationMessengerCreateInfo;
            }
            else
            {
                createInfo.enabledLayerCount = 0;
                VX_CORE_WARN("Vulkan: Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled");
            }

        }
        VK_CHECK_RESULT(vkCreateInstance(&createInfo, nullptr, &m_Instance));
        VX_CORE_INFO("VulkanInstance: Instance created successfully.");

        if(m_enableValidation)
        {
            vkclass::debug::setupValidation(m_Instance);
        }
    }

}
