#include "VulkanDebug.hpp"

namespace vkclass
{
    namespace debug
    {
        VkDebugUtilsMessengerEXT validationMessenger = VK_NULL_HANDLE;
        void setupValidation(VkInstance instance)
        {
            auto validationCreateFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                instance,
                "vkCreateDebugUtilsMessengerEXT"
            );
            if (validationCreateFunc != nullptr)
            {
                VkDebugUtilsMessengerCreateInfoEXT createInfo;
                setupValidationMessengerCI(createInfo);
                if (validationCreateFunc(instance, &createInfo, nullptr, &validationMessenger) != VK_SUCCESS)
                {
                    VX_CORE_ERROR("Vulkan: Failed to set up vaildation messenger.");
                    throw std::runtime_error("Vulkan: Failed to set up vaildation messenger.");
                }
            }
            
            VX_CORE_INFO("Vulkan: Validation messenger is set up");
        }
    
        void clearValidation(VkInstance instance)
        {
            auto validationClearFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,"vkDestroyDebugUtilsMessengerEXT");
            if( validationClearFunc != nullptr )
            {
                validationClearFunc(instance, validationMessenger, nullptr);
            }
            
            VX_CORE_INFO("Vulkan: Validation messenger destroyed");
        }
    
        void setupValidationMessengerCI(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
        {
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = ValidationCallback;
            createInfo.pUserData = nullptr; // Optional
        }
    
        VkBool32 ValidationCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
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
    }
}
