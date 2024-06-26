#pragma once
#include "VortexPCH.hpp"
#include "Core.hpp"
#include "Events/Event.hpp"

namespace VX
{
    struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;
        
        WindowProps(const std::string& title = "VortexEngine",
                    unsigned int width = 1280,
                    unsigned int height = 720
                    )
        : Title(title), Width(width), Height(height)
        {
        }
    };

    // Interface represeting a desktop system based window
    class VX_API Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;
        
        virtual ~Window() {}
        
        virtual void OnUpdateStart() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnUpdateEnd() = 0;
        
        virtual void Resize(unsigned int width, unsigned int height) = 0;
        virtual void OnClose() = 0;
        
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        
        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enable) = 0;
        virtual bool IsVSync() const = 0;
        
        virtual void* GetNativeWindow() const = 0;
        
        static Scope<Window> Create(const WindowProps& props = WindowProps());
    };

}
