#include "VortexPCH.hpp"
#include "Window.hpp"
#include "Platform/MacWindow.hpp"
#include "Platform/WindowsWindow.hpp"

namespace VX
{
    Window* Window::Create(const WindowProps& props)
    {
    #if defined(_MSC_VER)
        VX_CORE_INFO("Creating WindowsOS window...");
        return new WindowsWindow(props);
    #else
        VX_CORE_INFO("Creating MacOS window");
        return new MacWindow(props);
    #endif
        return nullptr;
    }
}