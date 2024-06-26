#include "VortexPCH.hpp"
#include "GraphicsContext.hpp"
#include "Platform/GraphicsAPI/Vulkan/VulkanContext.hpp"

namespace VX
{  
    Scope<GraphicsContext> GraphicsContext::Create(GLFWwindow* window)
    {
        if (window == nullptr)
        {
            VX_CORE_ERROR("Window not created, therefore graphics context does not create.");
            return nullptr;
        }
        return CreateScope<VulkanContext>(window);
    }
}