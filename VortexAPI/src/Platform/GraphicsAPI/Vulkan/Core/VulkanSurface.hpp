#pragma once
#include "VortexPCH.hpp"

#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>

#include "VulkanInstance.hpp"

namespace vkclass
{
    /** @brief Abstract type of surface for rendering*/
    class VulkanSurface
    {
    public:
        const VkSurfaceKHR& Surface = m_surface;

        explicit VulkanSurface(GLFWwindow* window, vkclass::VulkanInstance* instance);
        ~VulkanSurface();
        
        void SetExtent(unsigned int width, unsigned int height);
        VkExtent2D GetExtent();
        bool ValidateExtent();
        
    private:
        GLFWwindow* m_window;
        
        VkSurfaceKHR m_surface;
        vkclass::VulkanInstance* m_instance;
        
        VkExtent2D m_extent;
    };
}

