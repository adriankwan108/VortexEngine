#pragma once

#include "VortexPCH.hpp"
#include "Vortex/Core.hpp"

namespace VX
{
    class VX_API Input
    {
    public:
        static bool IsKeyPressed(int keycode) { return s_instance->IsKeyPressed(keycode); }
        
    protected:
        virtual bool IsKeyPressed(int keycode) = 0;
        
    private:
        // singleton
        static Input* s_instance;
    };
}
