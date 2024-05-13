#pragma once

#include "VortexPCH.hpp"
#include "RendererAPI.hpp"


namespace VX
{
    /*
     *  This is a static wrapper
     */
    class VX_API RenderCommand
    {
    public:
        static void Init()
        {
            // this is called in Renderer::Init(), which is after context created
            VX_CORE_INFO("Render Command: Initiating...");
            s_RendererAPI->Init();
        }
        
        static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }
        
        static void DrawIndexed(const Ref<VX::VertexArray>& vertexArray)
        {
            s_RendererAPI->DrawIndexed(vertexArray);
        }
    private:
        static Scope<VX::RendererAPI> s_RendererAPI;
    };
}
