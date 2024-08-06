#pragma once

#include "VortexPCH.hpp"
#include <memory>
#include "Renderer/Buffer.hpp"

namespace VX
{
    // api agnostic interface
    class VertexArray
    {
    public:
        static Ref<VertexArray> Create();
        virtual ~VertexArray() = default;
        
        // the binding serves for different concept for different API, but this will be called on update
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        void AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer);
        void SetIndexBuffer (Ref<IndexBuffer>& indexBuffer);
        
        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() { return m_vertexBuffers; }
        virtual const Ref<IndexBuffer>& GetIndexBuffer() { return m_indexBuffer; }
        
    protected:
        std::vector<Ref<VertexBuffer>> m_vertexBuffers;
        Ref<IndexBuffer> m_indexBuffer;

        virtual void AddApiVertexBuffer(Ref<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetApiIndexBuffer(Ref<IndexBuffer>& indexBuffer) = 0;
    };
}
