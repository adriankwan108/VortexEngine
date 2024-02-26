#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"

namespace vkclass
{
    namespace debug
    {
        
        void setupValidation(VkInstance instance);
        void clearValidation(VkInstance instance);
        void setupValidationMessengerCI(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        VkBool32 ValidationCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                    void* pUserData);
    }
}
