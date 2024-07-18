#pragma once

#include "VortexPCH.hpp"
#include "Shader.hpp"

namespace VX
{
    /*
    * API-agnostic Material System
    * Generally, it obtains the reference of textures, parameters, 
    *                           shader effects (preprocessed api-agnostic shaders), 
    *                           and related functions
    */
    class Material
    {
    public:
        VX::Ref<Material> Create();
        virtual ~Material() = default;

        virtual void Bind() = 0;

        void SetShaderEffect(Ref<ShaderEffect> shaderEffect);

    protected:
        // vector of texture
        Ref<ShaderEffect> m_shaderEffect;
    };

    // TODO: material cache
}
