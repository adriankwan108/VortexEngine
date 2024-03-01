#include "VulkanSurface.hpp"

namespace vkclass
{
    VulkanSurface::VulkanSurface(GLFWwindow* window, VkInstance instance):m_window(window),m_instance(instance)
    {
        // create surface
        if (glfwCreateWindowSurface(instance, window, nullptr, &m_surface) != VK_SUCCESS)
        {
            VX_CORE_ERROR("Vulkan Surface: Failed to create surface!");
            throw std::runtime_error("failed to create window surface!");
        }else
        {
            VX_CORE_INFO("Vulkan Surface: Surface created.");
        }
    }

    VulkanSurface::~VulkanSurface()
    {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        VX_CORE_INFO("Vulkan: Surface destroyed.");
    }
}
