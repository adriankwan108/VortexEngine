#pragma once
#include "VortexPCH.hpp"

namespace VX
{
    enum class ShaderDataType
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool, U32
    };

    static uint32_t ShaderDataTypeSize(VX::ShaderDataType type)
    {
        switch (type)
        {
            case VX::ShaderDataType::Float:    return 4;
            case VX::ShaderDataType::Float2:   return 4 * 2;
            case VX::ShaderDataType::Float3:   return 4 * 3;
            case VX::ShaderDataType::Float4:   return 4 * 4;
            case VX::ShaderDataType::Mat3:     return 4 * 3 * 3;
            case VX::ShaderDataType::Mat4:     return 4 * 4 * 4;
            case VX::ShaderDataType::Int:      return 4;
            case VX::ShaderDataType::Int2:     return 4 * 2;
            case VX::ShaderDataType::Int3:     return 4 * 3;
            case VX::ShaderDataType::Int4:     return 4 * 4;
            case VX::ShaderDataType::Bool:     return 1;
            case VX::ShaderDataType::U32:      return 4;
            default: VX_CORE_ASSERT(false, "Layout: Unknown Shader data type.");
        }
    }

    struct ShaderElement
    {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        
        ShaderElement() = default;
        ShaderElement(ShaderDataType type, const std::string& name)
            : Type(type), Name(name), Size(ShaderDataTypeSize(type))
        {
        }
    };

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
        
    };
}
