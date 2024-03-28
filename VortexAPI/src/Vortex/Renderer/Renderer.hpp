#pragma once

#include "VortexPCH.hpp"
#include "RendererAPI.hpp"
#include "VertexArray.hpp"

namespace VX
{
    /*
    *   This should manage high-level constructs, i.e. scene data, mesh
    */
    class Renderer
    {
    public:
        static void BeginScene();
        static void EndScene();
        
        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
        
        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };
}
