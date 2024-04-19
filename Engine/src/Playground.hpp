#pragma once

#include <Vortex.hpp>

class Playground : public VX::Layer
{
public:

    Playground();
    ~Playground() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnEvent(VX::Event& event) override;
    
    void UpdateUniformBuffer();
    
private:
    // prepare resources here
    std::shared_ptr<VX::Shader> m_basicShader;
    std::shared_ptr<VX::VertexArray> m_vertexArray;
    std::shared_ptr<VX::VertexBuffer> m_vertexBuffer;
    std::shared_ptr<VX::IndexBuffer> m_indexBuffer;
    
    Geometry::Uniform_MVP mvp{};
    
    VX::OrthographicCamera m_camera;
};

