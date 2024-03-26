#pragma once

#include "VortexPCH.hpp"

namespace vkclass
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None    = 0,
            Vulkan  = 1,
            DX12    = 2,    // will implement after a real game published
            OpenGL  = 3     // not gonna happen unless there is a huge update on OpenGL
        };
    public:
        virtual ~RendererAPI() = default;

        // ops
        // drawIndexed
        // drawlines

        static API GetAPI() { return s_API; }
        static RendererAPI* Create();
    private:
        static API s_API;
    };
}