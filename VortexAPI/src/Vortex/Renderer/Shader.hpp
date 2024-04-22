#pragma once

#include "VortexPCH.hpp"
#include "ShaderLayout.hpp"

namespace VX
{
    // api agnostic
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;
//        virtual void SetPipeline(ShaderLayout layout) = 0;

        virtual void SetVertexLayout(VX::VertexShaderLayout layout) = 0;
        virtual void SetUniformLayout(VX::UniformShaderLayout layout) = 0;
        virtual void SetDynamicLayout(VX::UniformShaderLayout layout) = 0;
        virtual void SetPushConstant(VX::UniformShaderLayout layout) = 0;
        virtual void Prepare() = 0;
        
        virtual const std::string& GetName() const = 0;
        
        static Shader* Create(const std::string& name, const std::string& vertexFilePath, const std::string& fragFilePath);
    };
}
