#pragma once

#include "VortexPCH.hpp"
#include "Vortex/Core.hpp"

namespace VX
{
    class VX_API Input
    {
    public:
        Input() {};
        ~Input() = default;

        inline static bool IsKeyPressed(int keycode) { return s_instance->IsKeyPressedFn(keycode); }
        inline static bool IsMouseButtonPressed(int button) { return s_instance->IsMouseButtonPressedFn(button); }
        inline static std::pair<float, float> GetMousePosition() { return s_instance->GetMousePositionFn(); }
        inline static float GetMouseX() { return s_instance->GetMouseXFn(); }
        inline static float GetMouseY() { return s_instance->GetMouseYFn(); }

        static void Set(Input* instance);
    protected:
        virtual bool IsKeyPressedFn(int keycode) = 0;
        virtual bool IsMouseButtonPressedFn(int keycode) = 0;
        virtual std::pair<float, float> GetMousePositionFn() = 0;
        virtual float GetMouseXFn() = 0;
        virtual float GetMouseYFn() = 0;

    private:
        // singleton
        static Input* s_instance;
    };
}
