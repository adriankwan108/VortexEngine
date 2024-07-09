#pragma once
#include "VortexPCH.hpp"

namespace VX
{
    /*
    class ShaderLayout
    {
    public:
        ShaderLayout(){}
        virtual ~ShaderLayout() = default;
        ShaderLayout(std::initializer_list<ShaderElement> elements):m_Elements(elements){}
        
        const std::vector<ShaderElement>& Elements = m_Elements;
        
        ShaderLayout& operator=(const ShaderLayout& other)
        {
            if(this != &other)
            {
                m_Elements = other.m_Elements;
            }
            return *this;
        }
    protected:
        std::vector<ShaderElement> m_Elements;
    };

    class VertexShaderLayout : public ShaderLayout
    {
    public:
        VertexShaderLayout() = default;
        VertexShaderLayout(std::initializer_list<ShaderElement> elements) : ShaderLayout(elements)
        {
            for(const auto& element : m_Elements)
            {
                m_Stride += element.Size;
            }
        }
        ~VertexShaderLayout() = default;
        uint32_t GetStride() const { return m_Stride; }
    private:
        uint32_t m_Stride = 0;
    };

    class UniformShaderLayout : public ShaderLayout
    {
    public:
        UniformShaderLayout(std::initializer_list<ShaderElement> elements) : ShaderLayout(elements)
        {
            
        }
        
        ~UniformShaderLayout() = default;
    private:
        
    };*/
}
