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
        virtual ~Material() = default;

        virtual void Bind() = 0;
        virtual void Render() = 0;

    private:
        // vector of texture
        // std::vector<VX::Ref<ShaderEffect>> m_effects;
    };

    // TODO: material cache
}