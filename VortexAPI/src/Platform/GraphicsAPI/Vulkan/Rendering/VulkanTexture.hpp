#pragma once

#include "VortexPCH.hpp"
#include "Vortex/Renderer/Texture.hpp"
#include "vulkan/vulkan.h"

namespace vkclass
{
    class VulkanTexture2D : public VX::Texture2D
    {
    public:
        VulkanTexture2D(const std::string& path);
        ~VulkanTexture2D();

        const std::string& GetPath() override { return m_path; }

        virtual uint32_t GetWidth()  const override { return m_Width; }
        virtual uint32_t GetHeight() const override { return m_Height; }

    private:
        std::string m_path = "None";
        uint32_t m_Width, m_Height;
    };
}

