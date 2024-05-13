#pragma once

#include "VortexPCH.hpp"
#include <memory>
#include "Renderer/Buffer.hpp"

namespace VX
{
    /*
     *  Reminder: Every resources like vertex buffer, texture, shader, e.t.c.,
                  are going to be reference counted, thus => shared pointers.
                  For performance, should use an intrusive reference counting system,
                  but shared_ptr should not be inline in memory
     */

    // api agnostic interface
    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;
        
        // the binding serves for different concept for different API, but this will be called on update
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        // TODO: use ref,
        virtual void AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer (Ref<IndexBuffer>& indexBuffer) = 0;
        
        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const = 0;
        
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
        
        static Ref<VertexArray> Create();
    };
}
