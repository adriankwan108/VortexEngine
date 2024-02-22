#pragma once
#include "VortexPCH.hpp"
#include "Renderer/GraphicsContext.hpp"
#include <GLFW/glfw3.h>
#include "vulkan/vulkan.h"

namespace VX
{
    class VulkanContext : public GraphicsContext
    {
    public:
        VulkanContext(GLFWwindow* window);
        ~VulkanContext();

        virtual void Init() override;
        virtual void Display() override;

    private:
        uint32_t m_glfwExtensionCount = 0;
        const char** m_glfwExtensions;
        
        // extensions supported by current hardware
        std::vector<std::string> m_supportedInstanceExtensions;
        
        // extensions required by os
        std::vector<const char*> m_requiredDeviceExtensions;
        std::vector<const char*> m_requiredInstanceExtensions;
        
        // extensions to be enabled
        std::vector<const char*> m_enabledInstanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
        
    private: // may be required to be exposed for imgui
        VkInstance m_Instance = VK_NULL_HANDLE;
        // physical device
        // device
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
        
    private:
        VkResult createInstance(bool enableValidation);
    };
}
