#pragma once
#include "VortexPCH.hpp"

namespace VX
{
    enum class ShaderDataType
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    struct BufferElement
    {
        std::string Name;
        ShaderDataType Type;
        
        BufferElement() = default;
        BufferElement(ShaderDataType type, const std::string& name)
            : Type(type), Name(name)
        {
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout(){}
        virtual ~BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> elements):m_Elements(elements){}
        
        const std::vector<BufferElement>& Elements = m_Elements;
    protected:
        std::vector<BufferElement> m_Elements;
    };

    // api agnostic interface
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;
        
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
        
        static IndexBuffer* Create(void* data, uint64_t size);
    };

}
