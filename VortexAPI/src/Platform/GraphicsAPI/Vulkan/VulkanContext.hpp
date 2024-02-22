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

        virtual void Init() override;
        virtual void Display() override;

    private:
        
    };
}