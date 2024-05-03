#pragma once
#include "VortexPCH.hpp"

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Geometry
{
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;
    };

    struct Uniform_VP
    {
        glm::mat4 view;
        glm::mat4 proj;
    };
}
