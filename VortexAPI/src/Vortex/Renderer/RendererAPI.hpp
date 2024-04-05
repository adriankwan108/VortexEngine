#pragma once

#include "VortexPCH.hpp"
#include <glm/glm.hpp>
#include "VertexArray.hpp"

namespace VX
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
        
        virtual void Init() = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
        virtual void Clear() = 0;

        static API GetAPI() { return s_API; }
        static RendererAPI* Create();
    private:
        static API s_API;
    };
}
