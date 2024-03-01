#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>

namespace vkclass
{
    class VulkanSurface
    {
    public:
        const VkSurfaceKHR& Surface = m_surface;
        GLFWwindow* Window = m_window;

        explicit VulkanSurface(GLFWwindow* window, VkInstance instance);
        ~VulkanSurface();
        
    private:
        GLFWwindow* m_window;
        
        VkSurfaceKHR m_surface;
        VkInstance m_instance;
    };
}

