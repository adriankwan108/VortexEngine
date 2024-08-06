#include "Renderer.hpp"
#include "RenderCommand.hpp"

//#define GLM_ENABLE_EXPERIMENTAL
//#include "glm/gtx/string_cast.hpp"

namespace VX
{
    Scope<Renderer::SceneData> Renderer::s_sceneData = CreateScope<Renderer::SceneData>();

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

    void Renderer::BeginScene(std::shared_ptr<OrthographicCamera> camera)
    {
        // VX_CORE_INFO("Renderer: Beginning Scene...");
        s_sceneData->viewProjection = camera->GetViewProjectionMatrix();
        
    }

    void Renderer::EndScene()
    {
//        VX_CORE_INFO("Renderer: Ending Scene...");
    }

    //void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray)
    //{
    //    // VX_CORE_TRACE("Renderer:: Submiting...");
    //    /* update per-material / per-object data here */
    //    shader->Bind();
    //    
    //    if(vertexArray != nullptr)
    //    {
    //        vertexArray->Bind();
    //        RenderCommand::DrawIndexed(vertexArray);
    //    }
    //    // VX_CORE_TRACE("Renderer:: Submited...");
    //}

    void Renderer::Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray)
    {
        material->Bind();

        if(vertexArray != nullptr)
        {
            vertexArray->Bind();
            RenderCommand::DrawIndexed(vertexArray);
        }
    }
}
