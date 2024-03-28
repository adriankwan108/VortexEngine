#pragma once

#include "VortexPCH.hpp"
#include "Buffer.hpp"

namespace VX
{
    // api agnostic
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;
        virtual void SetPipeline(BufferLayout layout) = 0;

        virtual const std::string& GetName() const = 0;
        
        static Shader* Create(const std::string& name, const std::string& vertexFilePath, const std::string& fragFilePath);
    };
}
