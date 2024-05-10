#include "Playground.hpp"

#include <chrono>

Playground::Playground()
    : VX::Layer("Playground")
{
    VX_INFO("{0}: Creating...", GetName());
    
    m_camera = std::shared_ptr<VX::OrthographicCamera>(
        VX::OrthographicCamera::Create(-1.6f, 1.6f, -0.9f, 0.9f)
    );
    
    // TODO: Shader to BufferLayout(ShaderLayout) transformer (Reflection)
    // define shader layout
    VX::VertexShaderLayout layout = {
        {VX::ShaderDataType::Float2, "pos"},
        {VX::ShaderDataType::Float3, "color"}
    };
    
    VX::UniformShaderLayout viewProjLayout = {
        {VX::ShaderDataType::Mat4, "view"},
        {VX::ShaderDataType::Mat4, "proj"}
    };

    VX::UniformShaderLayout modelLayout = {
        {VX::ShaderDataType::Mat4, "model"},
    };
    
    // simulate vertices data (counter-clockwise, left-hand)
    std::vector<Geometry::Vertex> vertices =
    {
        {{0.0f, 0.5f}, {1.0f, 0.0f, 0.0f}},     // top
        {{-0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},   // bottom, left
        {{0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}     // bottom, right
    };
    
    // simulate index data
    std::vector<uint16_t> triangleIndices = {
        0, 1, 2
    };
    
    m_vertexArray = std::shared_ptr<VX::VertexArray>(
        VX::VertexArray::Create()
    );
    
    // properly use a material system to create pipeline would be better
    m_basicShader = std::shared_ptr<VX::Shader>(
        VX::Shader::Create("Triangle", "Resources/VortexAPI/shaders/vert.spv", "Resources/VortexAPI/shaders/frag.spv")
    );
    m_basicShader->SetVertexLayout(layout); // for getting attributes, bindings
    
    m_basicShader->SetGlobalLayout(0, viewProjLayout);
    
    m_basicShader->Prepare();
    
    m_vertexBuffer = std::shared_ptr<VX::VertexBuffer>(VX::VertexBuffer::Create(vertices.data(), MEM_SIZE(vertices)));
    m_vertexBuffer->SetLayout(layout); // for getting stride
    
    m_indexBuffer = std::shared_ptr<VX::IndexBuffer>(VX::IndexBuffer::Create(triangleIndices.data(), MEM_SIZE(triangleIndices), triangleIndices.size()));
    
    m_vertexArray->AddVertexBuffer(m_vertexBuffer);
    m_vertexArray->SetIndexBuffer(m_indexBuffer);
    
    
    VX_INFO("{0}: Created", GetName());
}

void Playground::OnAttach()
{
    VX_INFO("{0}: Attaching...", GetName());
    VX_INFO("{0}: Attached", GetName());
}

void Playground::OnDetach()
{
    VX_INFO("{0}: Detaching...", GetName());

    VX_INFO("{0}: Detached", GetName());
}

void Playground::OnUpdate(VX::Timestep ts)
{
    VX::Renderer::BeginScene(m_camera);
    
    // m_camera->Update();
    m_camera->SetPosition({0.5f, 0.5f, 0.0f});
    m_camera->SetRotation(45.0f);
    
    // set clear color
    VX::RenderCommand::SetClearColor(glm::vec4(0.11f, 0.12f, 0.13f, 1.0f));
    
    UpdateUniformBuffer();
    
    VX::Renderer::Submit(m_basicShader ,m_vertexArray);
    
    VX::Renderer::EndScene();
    // renderer::Flush
}

void Playground::OnEvent(VX::Event& event)
{
    
}

void Playground::UpdateUniformBuffer()
{
    // update data
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
//    mvp.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    mvp.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    mvp.proj = glm::perspective(glm::radians(45.0f), 1920.0f/ 1080.0f, 0.1f, 10.0f); // should use swapchain extent
//    mvp.proj[1][1] *= -1;
    
    // update buffer
}
