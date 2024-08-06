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

        const uint64_t& GetStride() const { return m_stride; }

        static Ref<VertexBuffer> Create(void* data, uint64_t totalSize, uint64_t stride);

    protected:
        uint64_t m_stride;
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual void SetData(void* data, uint64_t size) = 0;
        
    public:
        struct DrawCmdInfo{
            uint32_t indexCount;
            uint32_t firstIndex;
            int32_t vertexOffset;
            
            int32_t offsetX;
            int32_t offsetY;
            uint32_t width;
            uint32_t height;
        };
        
        virtual void AddDrawCmd(uint32_t indexCount, uint32_t firstIndex = 0, int32_t vertexOffset = 0,
                                int32_t offsetX = 0,
                                int32_t offsetY = 0,
                                uint32_t width = 0,
                                uint32_t height = 0);
        virtual std::vector<DrawCmdInfo> GetDrawInfoList() const { return m_drawInfoList; }
        
        static Ref<IndexBuffer> Create(void* data, uint64_t size);
    private:
        std::vector<DrawCmdInfo> m_drawInfoList;
    };

    
    class UniformBuffer
    {
    public:
        virtual ~UniformBuffer() = default;
        
        virtual void Update(void* data, uint64_t size) = 0;
        
        static Ref<UniformBuffer> Create(uint64_t size);
    };
}
