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
        BufferLayout(std::initializer_list<BufferElement> elements):m_Elements(elements){}
        
        const std::vector<BufferElement>& Elements = m_Elements;
    protected:
        std::vector<BufferElement> m_Elements;
    };

    // this just works as an interface
    class VertexBuffer
    {
    public:
        virtual void SetLayout(const BufferLayout& layout) = 0;
    };

}
