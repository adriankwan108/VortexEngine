#pragma once
#include "VortexPCH.hpp"

namespace VX
{
    enum class ShaderDataType
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
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
            default: VX_CORE_ASSERT(false, "Layout: Unknown Shader data type.");
        }
    }

    struct BufferElement
    {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        
        BufferElement() = default;
        BufferElement(ShaderDataType type, const std::string& name)
            : Type(type), Name(name), Size(ShaderDataTypeSize(type))
        {
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout(){}
        virtual ~BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> elements):m_Elements(elements)
        {
            for(const auto& element : m_Elements)
            {
                m_Stride += element.Size;
            }
        }
        
        const std::vector<BufferElement>& Elements = m_Elements;
        uint32_t GetStride() const { return m_Stride; }
        
        BufferLayout& operator=(const BufferLayout& other)
        {
            if(this != &other)
            {
                m_Elements = other.m_Elements;
            }
            return *this;
        }
    protected:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    // api agnostic interface
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;
        
        // the binding serves for different concept for different API, but this will be called when added to vertex array
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual void SetData(const void* data, uint64_t size) = 0;
        
        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;
        
        static VertexBuffer* Create(void* data, uint64_t size);
    };

    // api agnostic interface
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;
        
        static IndexBuffer* Create(void* data, uint64_t size, unsigned long count);
    };

}
