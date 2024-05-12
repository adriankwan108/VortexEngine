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
    std::shared_ptr<VX::Shader> m_basicShader;
    std::shared_ptr<VX::VertexArray> m_vertexArray;
    std::shared_ptr<VX::VertexBuffer> m_vertexBuffer;
    std::shared_ptr<VX::IndexBuffer> m_indexBuffer;
    
    Geometry::Uniform_VP mvp{};
    
    std::shared_ptr<VX::OrthographicCamera> m_camera;

    glm::vec3 m_cameraPosition = {0.0f, 0.0f, 0.0f};
    float m_CameraMoveSpeed = 1.0f;
};

