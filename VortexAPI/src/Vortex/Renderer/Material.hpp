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
        static VX::Ref<Material> Create(Ref<ShaderEffect> shaderEffect);
        virtual ~Material() = default;

        virtual void Bind() = 0;
    };

    // TODO: material cache
}
