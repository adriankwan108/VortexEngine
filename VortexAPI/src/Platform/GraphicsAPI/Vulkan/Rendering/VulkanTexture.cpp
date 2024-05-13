#include "VortexPCH.hpp"
#include "VulkanTexture.hpp"

#include "stb_image.hpp"

namespace vkclass
{
    VulkanTexture2D::VulkanTexture2D(const std::string& path)
        :m_path(path)
    {
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        VX_CORE_ASSERT( pixels,"VulkanTexture2D: Failed to load image!");
        m_Width  = texWidth;
        m_Height = texHeight;

        // ubo

        stbi_image_free(pixels);
    }

    VulkanTexture2D::~VulkanTexture2D()
    {

    }
}
