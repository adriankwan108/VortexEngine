#pragma once
#include "VortexPCH.hpp"
#include "ShaderLayout.hpp"

/*
 *  Below classes are api-agnostic interfaces
 */

namespace VX
{
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;
        
        // the binding serves for different concept for different API, but this will be called when added to vertex array
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual void SetData(void* data, uint64_t size) = 0;
        
        virtual void SetLayout(const VertexShaderLayout& layout) = 0;
        virtual const VertexShaderLayout& GetLayout() const = 0;
        
        static Ref<VertexBuffer> Create();
        static Ref<VertexBuffer> Create(void* data, uint64_t size);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;
        
        virtual void SetData(void* data, uint64_t size, unsigned long count) = 0;
        
        static Ref<IndexBuffer> Create();
        static Ref<IndexBuffer> Create(void* data, uint64_t size, unsigned long count);
    };

    
    class UniformBuffer
    {
    public:
        virtual ~UniformBuffer() = default;
        
        virtual void Update(void* data, uint64_t size) = 0;
        
        static Ref<UniformBuffer> Create(uint64_t size);
    };
}
