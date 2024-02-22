#pragma once
#include "VortexPCH.hpp"
#include <GLFW/glfw3.h>

namespace VX
{
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void Display() = 0;

        static GraphicsContext* Create(GLFWwindow* window = nullptr);
    };
}