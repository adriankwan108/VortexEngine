#pragma once

#include "VortexPCH.hpp"
#include "Core/Input.hpp"

namespace VX
{
    class WindowsInput : public Input
    {
    public:
        WindowsInput();
    protected:
        virtual bool IsKeyPressedFn(int keycode) override;
        virtual bool IsMouseButtonPressedFn(int keycode) override;
        virtual std::pair<float, float> GetMousePositionFn() override;
        virtual float GetMouseXFn() override;
        virtual float GetMouseYFn() override;
    };
}
