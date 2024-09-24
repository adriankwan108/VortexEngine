#pragma once

namespace VX
{
    // this is vulkan stuff
//    {
//        struct RTAttachmentDescription
//        {
//            // format
//            // samples
//            // load op
//            // store op
//        };
//
//        struct RTSubpassReference
//        {
//            // index
//            // layout
//        };
//
//        struct RTSubpassDescription
//        {
//            // bind point
//            // color    references
//
//            // optional:
//            // input    references
//            // resolve  references
//            // depth    references
//            // preserve references
//        };
//    }

    struct RTAttachmentSpecification
    {
        // texture* (format, samples...)
        // load, store operations
    };

    enum class SubpassHint
    {
        None = 0
    };

    struct RenderTargetSpecification
    {
        uint32_t Width = 0, Height = 0;
        bool IsSwapChainTarget = false;
        SubpassHint Hint = SubpassHint::None;
        
        // color    attachment
        
        // optional:
        // input    attachment
        // resolve  attachment
        // depth    attachment
        // preserve attachment
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

        static Ref<RenderTarget> Create(const std::string& name, const RenderTargetSpecification& spec);
    };
}
