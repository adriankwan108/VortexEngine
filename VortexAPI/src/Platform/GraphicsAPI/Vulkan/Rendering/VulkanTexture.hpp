#pragma once

#include "VortexPCH.hpp"
#include "Vortex/Renderer/Texture.hpp"
#include "vulkan/vulkan.h"
#include "Core/VulkanBuffer.hpp"
#include "Core/VulkanDevice.hpp"
#include "Core/VulkanCommandManager.hpp"
#include "Core/VulkanDescriptorManager.hpp"

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
        virtual void Bind(uint32_t slot) override;


        VX::Ref<VulkanDescriptor> GetDescriptor() const { return m_samplerDescriptor; }

        void Create();

        static void Init(VulkanDevice* device, VulkanCommandManager* cmdManager);
    private:
        static VulkanDevice* s_device;
        static VulkanCommandManager* s_cmdManager;
        
        std::string m_path = "None";
        uint32_t m_Width = 0, m_Height = 0;
        
        VkImage m_image;
        VkDeviceMemory m_imageMemory;
        VkImageLayout m_imageLayout;

        VkImageView m_imageView;
        VkSampler m_sampler;

        VX::Ref<VulkanDescriptor> m_samplerDescriptor;
        VkDescriptorImageInfo m_imgInfo;
        
    private:
        void createImage(vkclass::VulkanBuffer* stagingBuffer);
        void createImageView(VkImage* image, VkImageView* imageView);
        void createSampler(VkSampler* sampler);
    };
}

