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

    enum class RTAttachmentTextureFormat
    {
        None = 0,
        RGBA8_UINT
    };

    struct RTAttachmentTextureSpecification
    {
        RTAttachmentTextureSpecification() = default;
        RTAttachmentTextureSpecification(RTAttachmentTextureFormat format)
            :Format(format){}
        RTAttachmentTextureFormat Format = RTAttachmentTextureFormat::None;
        // sampling
    };

    enum class RTAttachmentOperations
    {
        DontCare = 0
    };
    

    /*
     Specify one buffer (inside not in flight framebuffer)'s format and what to do with the data inside this buffer
     */
    struct RTAttachmentSpecification
    {
        RTAttachmentSpecification(RTAttachmentTextureSpecification spec)
            :TextureSpecification(spec){}
  
        RTAttachmentTextureSpecification TextureSpecification;
        // load operation
        // store operation
    };

    /*
     Specify how buffers cooperated and be handled throughout rendering operations
     */
    enum class SubpassHint
    {
        None = 0
        // Deferred
    };

    /*
     Specify how many buffers are used, and how their contents should be handled throughout rendering operationzs
     */
    struct RenderTargetSpecification
    {
        uint32_t Width = 0, Height = 0;
        bool IsSwapChainTarget = false;
        
        SubpassHint Hint = SubpassHint::None;
        
        std::vector<RTAttachmentSpecification> colorAttachmentSpec;
        
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
