#pragma once
#include "VortexPCH.hpp"
#include "Renderer/GraphicsContext.hpp"

#include <GLFW/glfw3.h>
#include "vulkan/vulkan.h"
#include "Base/VulkanDevice.hpp"
#include "Base/VulkanTools.hpp"

namespace VX
{
    class VulkanContext : public GraphicsContext
    {
    public:
        VulkanContext(GLFWwindow* window);
        ~VulkanContext();

        virtual void Init() override;
        virtual void Display() override;

        static VKAPI_ATTR VkBool32 VKAPI_CALL ValidationCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

    private:
        uint32_t m_glfwExtensionCount = 0;
        const char** m_glfwExtensions;
        
        // extensions supported by current hardware
        std::vector<std::string> m_supportedInstanceExtensions = {};
        
        // extensions required by os
        std::vector<const char*> m_requiredDeviceExtensions = {};
        std::vector<const char*> m_requiredInstanceExtensions = {};
        
        // extensions to be enabled
        std::vector<const char*> m_enabledInstanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
        
    private: // vulkan objects that may be required to be exposed for imgui
        VkInstance m_Instance = VK_NULL_HANDLE;
        VkPhysicalDevice m_gpu = VK_NULL_HANDLE;
        VkDevice m_LogicalDevice = VK_NULL_HANDLE;
        // queue family
        // queue
        // descriptor pool
        // render pass
        // min image count
        // image count
        // msaa samples
        // pipeline cache
        // subpass
        // useDynamicRendering

    private: // vulkan objects that not required by imgui
        VkDebugUtilsMessengerEXT m_validationMessenger = VK_NULL_HANDLE;
        vkclass::VulkanDevice* m_VulkanDevice = nullptr;
        
    private:
        void createInstance(bool enableValidation);
        void pickDevice();

        void setupValidationMessenger(bool enableValidation, VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void destroyValidationMessenger(bool enableValidation);
    };
}
