#pragma once
#include "VortexPCH.hpp"
#include "vulkan/vulkan.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Geometry
{
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;
        
        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex); // number of bytes from one entry to the next
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // per-vertex data, TODO: change this for particle system
            return  bindingDescription ;
        }
        
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions()
        {
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
            attributeDescriptions[0].binding = 0; // tells vulkan which binding comes
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);
            
            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);
            return attributeDescriptions;
        }
    };
}
