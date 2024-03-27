#include "Playground.hpp"

Playground::Playground()
    : VX::Layer("Playground")
{
    // define shader layout
    VX::BufferLayout layout = {
        {VX::ShaderDataType::Float2, "pos"},
        {VX::ShaderDataType::Float3, "color"}
    };
    
    // create vertex array
    m_vertexArray = std::unique_ptr<VX::VertexArray>(
        VX::VertexArray::Create()
    );
    
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
    
    // create vertex buffer (data)
    m_vertexBuffer = std::unique_ptr<VX::VertexBuffer>(VX::VertexBuffer::Create(vertices.data(), MEM_SIZE(vertices)));
    m_vertexBuffer->SetLayout(layout);
    
//    m_vertexArray->AddVertexBuffer(m_vertexBuffer);
    // vertex array -> add vertex buffer
    
    // load shader
    m_basicShader = std::unique_ptr<VX::Shader>(
        VX::Shader::Create("Triangle", "Resources/VortexAPI/shaders/vert.spv", "Resources/VortexAPI/shaders/frag.spv")
    );
    
    VX_CORE_INFO("{0}: Created", GetName());
}

void Playground::OnAttach()
{
    VX_CORE_INFO("{0}: Attached", GetName());
}

void Playground::OnDetach()
{
    m_basicShader.reset(); // temp usage, should use a reference counter inside API
    m_vertexArray.reset();
    m_vertexBuffer.reset();
    VX_CORE_INFO("{0}: Detached", GetName());
}

void Playground::OnUpdate()
{
    // update camera
    
    // renderer cmd
    // set clear color
    // begin scene
    
    // submit (shader, vertex array)
}

void Playground::OnEvent(VX::Event& event)
{
    
}

