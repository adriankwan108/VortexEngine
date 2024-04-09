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
        virtual void DisplayStart() = 0;
        virtual void DisplayEnd() = 0;
        virtual void End() = 0;
        virtual void Resize(unsigned int width, unsigned int height) = 0;

        static GraphicsContext* Create(GLFWwindow* window = nullptr);
    };
}
