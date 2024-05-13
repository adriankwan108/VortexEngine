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

        virtual void Bind() = 0;
        virtual void UnBind() const = 0;
//        virtual void SetPipeline(ShaderLayout layout) = 0;

        virtual void SetVertexLayout(VX::VertexShaderLayout layout) = 0;
        
        // grouped by update frequency
        virtual void SetGlobalLayout    (int binding, VX::UniformShaderLayout layout) = 0; //   set 0: global (per frame)
        virtual void SetPassLayout      (int binding, VX::UniformShaderLayout layout) = 0; //   set 1: per pass (shadows)
        virtual void SetMaterialLayout  (int binding, VX::UniformShaderLayout layout) = 0; //   set 2: per material
        virtual void SetObjectLayout    (int binding, VX::UniformShaderLayout layout) = 0; //   set 3: per object (transform)
        
        virtual void Prepare() = 0;
        
        virtual const std::string& GetName() const = 0;
        
        static Ref<Shader> Create(const std::string& name, const std::string& vertexFilePath, const std::string& fragFilePath);
    };
}
