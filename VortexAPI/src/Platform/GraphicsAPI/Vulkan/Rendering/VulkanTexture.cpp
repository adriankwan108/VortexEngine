#include "VortexPCH.hpp"
#include "VulkanTexture.hpp"

#include "stb_image.hpp"

namespace vkclass
{
    VulkanDevice* VulkanTexture2D::s_device = nullptr;
    VulkanCommandManager* VulkanTexture2D::s_cmdManager = nullptr;

    VulkanTexture2D::VulkanTexture2D(const std::string& path)
        :m_path(path)
    {
        VX_CORE_INFO("Texture: Loading {0}", m_path);
        int texWidth, texHeight, texChannels;
        stbi_uc* pixelsData = stbi_load(m_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        VX_CORE_ASSERT( pixelsData,"VulkanTexture2D: Failed to load image!");
        m_Width  = static_cast<uint32_t>(texWidth);
        m_Height = static_cast<uint32_t>(texHeight);

        vkclass::VulkanBuffer stagingBuffer = vkclass::VulkanBuffer(pixelsData, imageSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        
        createImage(&stagingBuffer);

        stbi_image_free(pixelsData);
    }

    VulkanTexture2D::~VulkanTexture2D()
    {
        vkDestroySampler(s_device->LogicalDevice, m_sampler, nullptr);
        vkDestroyImageView(s_device->LogicalDevice, m_imageView, nullptr);
        vkDestroyImage(s_device->LogicalDevice, m_image, nullptr);
        vkFreeMemory(s_device->LogicalDevice, m_imageMemory, nullptr);
    }

    void VulkanTexture2D::Init(VulkanDevice* device, VulkanCommandManager* cmdManager)
    {
        s_device = device;
        s_cmdManager = cmdManager;
    }

    void VulkanTexture2D::createImage(vkclass::VulkanBuffer* stagingBuffer)
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = m_Width;
        imageInfo.extent.height = m_Height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        
        imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // only for graphics(transfer inclusive)
        
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.flags = 0; // Optional
        
        if (vkCreateImage(s_device->LogicalDevice, &imageInfo, nullptr, &m_image) != VK_SUCCESS)
        {
            VX_CORE_ERROR("VulkanTexture: Failed to create image!");
            throw std::runtime_error("failed to create image!");
        }
        
        VkDeviceSize allocSize;
        uint32_t memType;
        s_device->GetMemoryInfo(m_image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, allocSize, memType);
        
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = allocSize;
        allocInfo.memoryTypeIndex = memType;

        if (vkAllocateMemory(s_device->LogicalDevice, &allocInfo, nullptr, &m_imageMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate image memory!");
        }
        
        vkBindImageMemory(s_device->LogicalDevice, m_image, m_imageMemory, 0);

        s_cmdManager->TransitImageLayout(m_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        s_cmdManager->CopyBufferToImage(stagingBuffer->Buffer, m_image, m_Width, m_Height);
        s_cmdManager->TransitImageLayout(m_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }

    void VulkanTexture2D::createImageView()
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VK_CHECK_RESULT(vkCreateImageView(s_device->LogicalDevice, &viewInfo, nullptr, &m_imageView));
    }

    void VulkanTexture2D::createSampler()
    {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;

        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = s_device->DeviceProps.limits.maxSamplerAnisotropy;
        /*samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;*/

        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        VK_CHECK_RESULT(vkCreateSampler(s_device->LogicalDevice, &samplerInfo, nullptr, &m_sampler));
    }
}
