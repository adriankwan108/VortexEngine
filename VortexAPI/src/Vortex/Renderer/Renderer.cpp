#include "Renderer.hpp"
#include "RenderCommand.hpp"

namespace VX
{
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

    void Renderer::BeginScene()
    {
//        VX_CORE_INFO("Renderer: Beginning Scene...");
        
        // in future, it should make sure what shader is using,
        // getting the right uniforms
    }

    void Renderer::EndScene()
    {
//        VX_CORE_INFO("Renderer: Ending Scene...");
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray> &vertexArray)
    {
        VX_CORE_TRACE("Renderer:: Submiting...");
        shader->Bind();
        
        // RenderCommand::DrawIndex()
        VX_CORE_TRACE("Renderer:: Submited...");
    }
}
