#include "VortexPCH.hpp"
#include "WindowsWindow.hpp"
#include "Renderer/GraphicsContext.hpp"

#include "Vortex/Core/Input.hpp"
#include "WindowsInput.hpp"

namespace VX
{
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        VX_CORE_ERROR("GLFW Error: {0}, {1}", error, description);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
        Input::Set(new WindowsInput());
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

        VX_CORE_INFO("Creating window for {0}: {1},{2}", props.Title, props.Width, props.Height);

        if (s_GLFWInitialized == false)
        {
            int success = glfwInit();
            VX_CORE_ASSERT(success, "Could not initialize GLFW...");
            s_GLFWInitialized = true;
        }

        // if use Vulkan
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        
        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        
        // create graphics context
        m_GraphicsContext = GraphicsContext::Create(m_Window);
        
        // init graphics context, TODO: init with engine configs, i.e. vsync
        m_GraphicsContext->Init();

        // SetVSync(true);

        // set glfw callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                WindowResizeEvent resizeEvent(width, height);
                data.EventCallback(resizeEvent);
                data.Width = width;
                data.Height = height;
            });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent closeEvent;
                data.EventCallback(closeEvent);
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mode)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent pressedEvent(key, 0);
                    data.EventCallback(pressedEvent);
                    break;
                }

                case GLFW_RELEASE:
                {
                    KeyReleasedEvent releasedEvent(key);
                    data.EventCallback(releasedEvent);
                    break;
                }

                case GLFW_REPEAT:
                {
                    KeyPressedEvent repeatEvent(key, 1);
                    data.EventCallback(repeatEvent);
                    break;
                }

                default:
                {
                    break;
                }
                }
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }

                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }

                default:
                {
                    break;
                }
                }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseScrolledEvent event((float)xOffset, (float)yOffset);
                data.EventCallback(event);
            });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseMovedEvent event((float)xPos, (float)yPos);
                data.EventCallback(event);
            });
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void WindowsWindow::OnUpdateStart()
    {
        glfwPollEvents();
        m_GraphicsContext->DisplayStart();
    }

    void WindowsWindow::OnUpdate()
    {

    }

    void WindowsWindow::OnUpdateEnd()
    {
        // glfwSwapBuffers(m_Window);
        m_GraphicsContext->DisplayEnd();
    }

    void WindowsWindow::Resize(unsigned int width, unsigned int height)
    {
        m_GraphicsContext->Resize(width, height);
    }

    void WindowsWindow::OnClose()
    {
        m_GraphicsContext->End();
    }

    void WindowsWindow::SetVSync(bool enable)
    {
        m_GraphicsContext->SetVSync(enable);
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }
}
