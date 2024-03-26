#pragma once

#include "VortexPCH.hpp"
#include "RendererAPI.hpp"

namespace vkclass
{
    /*
    *   This should manage scene data and ops
    */
    class Renderer
    {
    public:
        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };
}