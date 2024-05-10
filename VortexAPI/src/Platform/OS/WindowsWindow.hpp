#pragma once
#include "Window.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/KeyEvent.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Renderer/GraphicsContext.hpp"

namespace VX
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdateStart() override;
        void OnUpdate() override;
        void OnUpdateEnd() override;
        
        void Resize(unsigned int width, unsigned int height) override;
        void OnClose() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        // window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;
        void* GetNativeWindow() const override { return m_Window; }

    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

    private:
        GLFWwindow* m_Window;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };
        
        WindowData m_Data;

        std::unique_ptr<GraphicsContext> m_GraphicsContext;
    };
}

