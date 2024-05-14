#pragma once

#include "VortexPCH.hpp"
#include "Vortex/Renderer/Texture.hpp"
#include "vulkan/vulkan.h"
#include "Core/VulkanBuffer.hpp"
#include "Core/VulkanDevice.hpp"

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

        static void Init(VulkanDevice* device);
    private:
        static VulkanDevice* s_device;
        
        std::string m_path = "None";
        uint32_t m_Width = 0, m_Height = 0;
        
        VkImage m_image;
        VkDeviceMemory m_imageMemory;
        VkImageLayout m_imageLayout;
        
    private:
        void createImage();
    };
}

