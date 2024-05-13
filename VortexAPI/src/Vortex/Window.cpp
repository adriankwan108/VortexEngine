#include "VortexPCH.hpp"
#include "Window.hpp"
#include "Platform/OS/MacWindow.hpp"
#include "Platform/OS/WindowsWindow.hpp"

namespace VX
{
    Scope<Window> Window::Create(const WindowProps& props)
    {
    #if defined(_MSC_VER)
        VX_CORE_INFO("Creating WindowsOS window...");
        return CreateScope<WindowsWindow>(props);
    #elif defined(__APPLE__)
        VX_CORE_INFO("Creating MacOS window");
        return CreateScope<MacWindow>(props);
    #else
        VX_CORE_ERROR("Create window terminates: Not supported OS...");
        return nullptr;
    #endif
    }
}
