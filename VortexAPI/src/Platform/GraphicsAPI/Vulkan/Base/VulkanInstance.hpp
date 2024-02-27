#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>

#include "VulkanDebug.hpp"
#include "VulkanTools.hpp"

namespace vkclass
{
    class VulkanInstance
    {
    public:
        VulkanInstance(VkApplicationInfo appInfo, bool enableValidation);
        ~VulkanInstance();
    
        const VkInstance& Instance = m_Instance;
        
    private:
        bool m_enableValidation = true;
        VkInstance m_Instance = VK_NULL_HANDLE;
        
        uint32_t m_glfwExtensionCount = 0;
        const char** m_glfwExtensions;
   
        // extensions supported by current hardware
        std::vector<std::string> m_supportedInstanceExtensions = {};
        // extensions required by OS
        std::vector<const char*> m_requiredInstanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
        // extensions that are enabled
        std::vector<const char*> m_enabledInstanceExtensions = {};
   
       private:
           void getSupportedExtensions();
           void getRequiredExtensions();
           void enableExtensions();
           void createInstance(VkApplicationInfo appInfo);
    };
}
