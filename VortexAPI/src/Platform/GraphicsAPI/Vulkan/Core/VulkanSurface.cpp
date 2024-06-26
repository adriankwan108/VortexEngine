#include "VulkanSurface.hpp"

namespace vkclass
{
    VulkanSurface::VulkanSurface(GLFWwindow* window, vkclass::VulkanInstance* instance):m_window(window),m_instance(instance)
    {
        // create surface
        if (glfwCreateWindowSurface(instance->Instance, window, nullptr, &m_surface) != VK_SUCCESS)
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
        vkDestroySurfaceKHR(m_instance->Instance, m_surface, nullptr);
        VX_CORE_INFO("Vulkan: Surface destroyed.");
    }

    void VulkanSurface::SetExtent(unsigned int width, unsigned int height)
    {
        m_extent =
        {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
    }

    VkExtent2D VulkanSurface::GetExtent()
    {
        return m_extent;
    }

    bool VulkanSurface::ValidateExtent()
    {
        if (m_extent.width == 0 || m_extent.height == 0)
        {
            glfwWaitEvents();
            //VX_CORE_TRACE("Vulkan Surface: Validate Extent: false");
            return false;
        }
        else
        {
            //VX_CORE_TRACE("Vulkan Surface: Validate Extent: true");
            return true;
        }
    }
}
