#include "VulkanContext.hpp"

namespace VX
{
    VulkanContext::VulkanContext(GLFWwindow* window)
    {
        m_glfwExtensions = glfwGetRequiredInstanceExtensions(&m_glfwExtensionCount);
    }

    VulkanContext::~VulkanContext()
    {
        vkDestroyInstance(m_Instance, nullptr);
        VX_CORE_INFO("Vulkan Instance destroyed.");
    }

    void VulkanContext::Init()
    {
        VX_CORE_INFO("Initiating Vulkan context...");
        if(createInstance(false) != VK_SUCCESS)
        {
            VX_CORE_ERROR("Failed to create instance!");
            throw std::runtime_error("Failed to create instance!");
        }
    }

    void VulkanContext::Display()
    {
        
    }

     VkResult VulkanContext::createInstance(bool enableValidation)
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
                 m_supportedInstanceExtensions.push_back(extension.extensionName);
                 VX_CORE_INFO("Support extension: {0}", extension.extensionName);
             }
         }
         
         // get all required extensions
#if (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
        // SRS - When running on iOS/macOS with MoltenVK, enable VK_KHR_get_physical_device_properties2 if not already enabled (required by VK_KHR_portability_subset)
        if (std::find(m_requiredInstanceExtensions.begin(), m_requiredInstanceExtensions.end(), VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) == enabledInstanceExtensions.end())
        {
            m_requiredInstanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        }
#endif
      
         for(uint32_t i = 0; i < m_glfwExtensionCount; i++) {
             m_requiredInstanceExtensions.emplace_back(m_glfwExtensions[i]);
         }

         
         // Enabled requested instance extensions
         if (m_requiredInstanceExtensions.size() > 0)
         {
             for (const char * requiredExtension : m_requiredInstanceExtensions)
             {
                 // Output message if requested extension is not available
                 if (std::find(m_supportedInstanceExtensions.begin(), m_supportedInstanceExtensions.end(), requiredExtension) == m_supportedInstanceExtensions.end())
                 {
                     VX_CORE_ERROR("Enabled instance extensions {0} is not present at instance level.", requiredExtension);
                 }else
                 {
                     VX_CORE_INFO("Instance extension: {0} enabled", requiredExtension);
                 }
                 m_enabledInstanceExtensions.push_back(requiredExtension);
             }
         }
         
         VkInstanceCreateInfo createInfo{};
         createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//         createInfo.pNext = NULL;
         createInfo.pApplicationInfo = &appInfo;
         
#if defined(__APPLE__)
//          SRS - When running on iOS/macOS with MoltenVK and VK_KHR_portability_enumeration is defined and supported by the instance, enable the extension and the flag
         if (std::find(m_supportedInstanceExtensions.begin(), m_supportedInstanceExtensions.end(), VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) != m_supportedInstanceExtensions.end())
         {
             m_enabledInstanceExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
             createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
         }
#endif
         
         createInfo.enabledExtensionCount = (uint32_t)m_enabledInstanceExtensions.size();
         createInfo.ppEnabledExtensionNames = m_enabledInstanceExtensions.data();
         createInfo.enabledLayerCount = 0;
         
         return vkCreateInstance(&createInfo, nullptr, &m_Instance);
    }
}
