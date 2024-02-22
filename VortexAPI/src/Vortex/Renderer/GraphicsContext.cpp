#include "VortexPCH.hpp"
#include "GraphicsContext.hpp"
#include "Platform/GraphicsAPI/Vulkan/VulkanContext.hpp"

namespace VX
{  
    GraphicsContext* GraphicsContext::Create(GLFWwindow* window)
    {
        if (window == nullptr)
        {
            VX_CORE_ERROR("Window not created, therefore graphics context does not create.");
            return nullptr;
        }
        return new VulkanContext(window);
    }
}