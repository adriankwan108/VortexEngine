#include "Renderer.hpp"
#include "RenderCommand.hpp"

//#define GLM_ENABLE_EXPERIMENTAL
//#include "glm/gtx/string_cast.hpp"

namespace VX
{
    Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData;

    void Renderer::Init()
    {
        VX_CORE_INFO("Renderer: initing...");
        // init command call
        RenderCommand::Init();
        
        // init 2d & 3d renderer
        VX_CORE_INFO("Renderer: initiated.");
    }

    void Renderer::ShutDown()
    {
        VX_CORE_INFO("Renderer: shuting down...");
        // shut down 2d & 3d renderer
        VX_CORE_INFO("Renderer: shut down.");
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        // VX_CORE_INFO("Renderer: Beginning Scene...");
        s_sceneData->viewProjection = camera.GetViewProjectionMatrix();
        
    }

    void Renderer::EndScene()
    {
//        VX_CORE_INFO("Renderer: Ending Scene...");
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray> &vertexArray)
    {
        // VX_CORE_TRACE("Renderer:: Submiting...");

        shader->Bind();
        
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
        // VX_CORE_TRACE("Renderer:: Submited...");
    }
}
