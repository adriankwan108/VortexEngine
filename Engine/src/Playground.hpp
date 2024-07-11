#pragma once

#include <Vortex.hpp>

class Playground : public VX::Layer
{
public:

    Playground();
    ~Playground() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(VX::Timestep ts) override;
    virtual void OnEvent(VX::Event& event) override;
    
private:
    // prepare resources here
    VX::Ref<VX::ShaderPass> m_basicShaderPass;
    
    VX::Ref<VX::VertexArray> m_vertexArray;
    VX::Ref<VX::VertexBuffer> m_vertexBuffer;
    VX::Ref<VX::IndexBuffer> m_indexBuffer;
    
    Geometry::Uniform_VP mvp{};
    
    VX::Ref<VX::OrthographicCamera> m_camera;
    
    VX::Ref<VX::Texture2D> m_texture;

    glm::vec3 m_cameraPosition = {0.0f, 0.0f, 0.0f};
    float m_CameraMoveSpeed = 1.0f;
};

