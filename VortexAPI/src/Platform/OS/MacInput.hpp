#pragma once

#include "VoretexPCH.hpp"
#include "Core/Input.hpp"

namespace VX
{
    class MacInput : public Input
    {
    protected:
        virtual bool IsKeyPressed(int keycode) override;
    };
}
