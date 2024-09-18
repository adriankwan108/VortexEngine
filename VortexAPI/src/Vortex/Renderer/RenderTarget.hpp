#pragma once

namespace VX
{
    struct RenderTargetSpecification
    {
        uint32_t Width = 0, Height = 0;
        bool IsSwapChainTarget = false;
    };

    /*
     API-agnostic output surface of rendering operations.
    */
    class RenderTarget
    {
        /*
        * OpenGL  : framebuffer object + attachment
        * Vulkan  : framebuffer object + render pass
        * DirectX : texture
        */
    public:
        virtual ~RenderTarget() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        static Ref<RenderTarget> Create(const RenderTargetSpecification& spec);
    };
}