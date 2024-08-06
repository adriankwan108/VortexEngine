#include "Playground.hpp"

Playground::Playground()
    : VX::Layer("Playground")
{
    VX_INFO("{0}: Creating...", GetName());
    
    m_camera = VX::OrthographicCamera::Create(-1.6f, 1.6f, -0.9f, 0.9f);

    // m_texture = VX::Texture2D::Create();
    // m_texture->LoadFromFile("Resources/VortexAPI/textures/statue-1275469_1280.jpg");
    
    // TODO: Shader to BufferLayout(ShaderLayout) transformer (Reflection)
    // define shader layout
    /*    
    VX::UniformShaderLayout viewProjLayout = {
        {VX::ShaderDataType::Mat4, "view"},
        {VX::ShaderDataType::Mat4, "proj"}
    };

    VX::UniformShaderLayout modelLayout = {
        {VX::ShaderDataType::Mat4, "model"},
    };*/
    
    // simulate vertices data (counter-clockwise, left-hand)
    std::vector<Geometry::Vertex> vertices =
    {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // left  bottom
        {{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}, // right bottom
        {{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // right top
        {{-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}  // left  top
    };
    
    // simulate index data
    std::vector<uint16_t> triangleIndices = {
        0, 1, 2, 2, 3, 0
    };
    
    m_vertexArray = VX::VertexArray::Create();
    
    // properly use a material system to create pipeline would be better
    //m_basicShader = VX::Shader::Create("Triangle", "Resources/VortexAPI/shaders/vert.spv", "Resources/VortexAPI/shaders/frag.spv");
    //m_basicShader->SetVertexLayout(layout); // for getting attributes, bindings
    //m_basicShader->SetGlobalLayout(0, viewProjLayout);
    //m_basicShader->SetTexture(m_texture);
    //m_basicShader->Prepare();
    
    // m_vertexBuffer = VX::VertexBuffer::Create(vertices.data(), MEM_SIZE(vertices));
    // m_vertexBuffer->SetLayout(layout); // for getting stride
    
    /*m_indexBuffer = VX::IndexBuffer::Create(triangleIndices.data(), MEM_SIZE(triangleIndices));
    m_indexBuffer->AddDrawCmd(static_cast<uint32_t>(triangleIndices.size()));
    
    m_vertexArray->AddVertexBuffer(m_vertexBuffer);
    m_vertexArray->SetIndexBuffer(m_indexBuffer);*/

    m_basicShaderPass = VX::ShaderPass::Create();
    m_vertShader = VX::Shader::Create("TriangleVert", "Resources/VortexAPI/shaders/vert.spv", VX::ShaderStage::Vertex);
    m_fragShader = VX::Shader::Create("TriangleFrag", "Resources/VortexAPI/shaders/frag.spv", VX::ShaderStage::Fragment);
    m_basicShaderPass->AddShader(m_vertShader);
    m_basicShaderPass->AddShader(m_fragShader);
    m_basicShaderPass->Prepare();
    
    m_basicShaderEffect = VX::ShaderEffect::Create(m_basicShaderPass);
    m_basicShaderEffect->SetRenderPass(VX::RenderPassStage::main);
    m_basicShaderEffect->Build();
    
    m_basicMaterial = VX::Material::Create(m_basicShaderEffect);
    // m_material->SetResource("name", *ref); // point the descriptor set to this resource

    m_vertexBuffer = VX::VertexBuffer::Create(vertices.data(), MEM_SIZE(vertices), sizeof(Geometry::Vertex));
    m_indexBuffer = VX::IndexBuffer::Create(triangleIndices.data(), MEM_SIZE(triangleIndices));
    VX_TRACE("AddDrawCmd: triangleIndices.size:{0}", static_cast<uint32_t>(triangleIndices.size()));
    m_indexBuffer->AddDrawCmd(static_cast<uint32_t>(triangleIndices.size()));

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
    // m_camera->SetPosition({0.5f, 0.5f, 0.0f});
    // m_camera->SetRotation(45.0f);

    // testing
    if (VX::Input::IsKeyPressed(VX::Key::A))
    {
        m_cameraPosition.x += m_CameraMoveSpeed * ts;
    }

    if (VX::Input::IsKeyPressed(VX::Key::D))
    {
        m_cameraPosition.x -= m_CameraMoveSpeed * ts;
    }

    if (VX::Input::IsKeyPressed(VX::Key::W))
    {
        m_cameraPosition.y += m_CameraMoveSpeed * ts;
    }

    if (VX::Input::IsKeyPressed(VX::Key::S))
    {
        m_cameraPosition.y -= m_CameraMoveSpeed * ts;
    }

    m_camera->SetPosition(m_cameraPosition);
    
    // set clear color
    VX::RenderCommand::SetClearColor(glm::vec4(0.11f, 0.12f, 0.13f, 1.0f));
    
     //VX::Renderer::Submit(m_basicShader ,m_vertexArray);
    VX::Renderer::Submit(m_basicMaterial, m_vertexArray);
    
    VX::Renderer::EndScene();
    // renderer::Flush
}

void Playground::OnEvent(VX::Event& event)
{
    //if (event.GetEventType() == VX::EventType::KeyPressed)
    //{
    //    VX::KeyPressedEvent& e = static_cast<VX::KeyPressedEvent&>(event);
    //    VX_TRACE("{0}", (char)e.GetKeyCode());
    //}
}
