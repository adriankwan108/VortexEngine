#include "Playground.hpp"

Playground::Playground()
    : VX::Layer("Playground")
{
    VX_INFO("{0}: Creating...", GetName());
    // TODO: Shader to BufferLayout(ShaderLayout) transformer (Reflection)
    // define shader layout
    VX::BufferLayout layout = {
        {VX::ShaderDataType::Float2, "pos"},
        {VX::ShaderDataType::Float3, "color"}
    };
    
    // simulate vertices data
    std::vector<Geometry::Vertex> vertices =
    {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };
    
    // simulate index data
    std::vector<uint16_t> triangleIndices = {
        0, 1, 2, 2, 3, 0
    };
    
    m_vertexArray = std::unique_ptr<VX::VertexArray>(
        VX::VertexArray::Create()
    );
    
    // properly use a material system to create pipeline would be better
    m_basicShader = std::shared_ptr<VX::Shader>(
        VX::Shader::Create("Triangle", "Resources/VortexAPI/shaders/vert.spv", "Resources/VortexAPI/shaders/frag.spv")
    );
    m_basicShader->SetPipeline(layout);
    
    m_vertexBuffer = std::shared_ptr<VX::VertexBuffer>(VX::VertexBuffer::Create(vertices.data(), MEM_SIZE(vertices)));
    // m_vertexBuffer->SetLayout(layout);
    m_indexBuffer = std::shared_ptr<VX::IndexBuffer>(VX::IndexBuffer::Create(triangleIndices.data(), MEM_SIZE(triangleIndices)));
    
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
    m_basicShader.reset(); // temp usage, should use a reference counter inside API
    m_vertexArray.reset();
    m_vertexBuffer.reset();
    m_indexBuffer.reset();
    VX_INFO("{0}: Detached", GetName());
}

void Playground::OnUpdate()
{
    // renderer::BeginScene(camera, environment)
    
    // update camera
    
    // set clear color

    
    // submit (mesh/vertices / material(shader, layout, should be inside of this) )
    
    // renderer::EndScene();
    
    // renderer::Flush
}

void Playground::OnEvent(VX::Event& event)
{
    
}

