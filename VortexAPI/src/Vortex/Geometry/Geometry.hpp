#pragma once
#include "VortexPCH.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Geometry
{
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;
        glm::vec2 texCoor;
    };

    struct Uniform_VP
    {
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };
}
