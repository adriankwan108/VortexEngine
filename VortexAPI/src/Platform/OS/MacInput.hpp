#pragma once

#include "VortexPCH.hpp"
#include "Core/Input.hpp"

namespace VX
{
    class MacInput : public Input
    {
    public:
        MacInput();
    protected:
        virtual bool IsKeyPressedFn(int keycode) override;
        virtual bool IsMouseButtonPressedFn(int keycode) override;
        virtual std::pair<float, float> GetMousePositionFn() override;
        virtual float GetMouseXFn() override;
        virtual float GetMouseYFn() override;
    };
}
