#pragma once

#include "VortexPCH.hpp"
#include "RendererAPI.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "OrthographicCamera.hpp"

#include <glm/glm.hpp>

namespace VX
{
    /*
    *   This should manage high-level constructs, i.e. scene data, mesh
    */
    class Renderer
    {
    public:
        static void Init();
        static void ShutDown();
        
        static void BeginScene(std::shared_ptr<OrthographicCamera> camera);
        static void EndScene();
        
        // static void Submit(const Ref<Shader>& shader ,const Ref<VertexArray>& vertexArray);
        
        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
        
    private:
        struct SceneData
        {
            glm::mat4 viewProjection;
        };
        
        static Scope<SceneData> s_sceneData;
    };
}
