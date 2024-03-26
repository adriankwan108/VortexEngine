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
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        // TODO: use ref,
        virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
        virtual void SetLayout(const BufferLayout layout) = 0;
        
        static VertexArray* Create(float* vertices, uint32_t size);
    };
}
