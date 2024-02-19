#include "VortexPCH.hpp"
#include "WindowsWindow.hpp"

namespace VX
{
    static bool s_GLFWInitialized = false;

    Window* Window::Create(const WindowProps& props)
    {
        // somewhere should have a detector of OS
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        VX_CORE_INFO("Creating window {0}: {1},{2}", props.Title, props.Width, props.Height);

        if (s_GLFWInitialized == false)
        {
            int success = glfwInit();
            VX_CORE_ASSERT(success, "Could not initialize GLFW...");
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        // only for opengl
        /*if(enabled)
        {
            glfwSwapInterval(1);
        }else
        {
            glfwSwapInterval(0);
        }*/
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }
}
