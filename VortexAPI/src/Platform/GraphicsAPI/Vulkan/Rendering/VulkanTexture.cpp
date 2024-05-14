#include "VortexPCH.hpp"
#include "VulkanTexture.hpp"

#include "stb_image.hpp"

namespace vkclass
{
    VulkanDevice* VulkanTexture2D::s_device = nullptr;

    VulkanTexture2D::VulkanTexture2D(const std::string& path)
        :m_path(path)
    {
        VX_CORE_INFO("Texture: Loading {0}", m_path);
        int texWidth, texHeight, texChannels;
        stbi_uc* pixelsData = stbi_load(m_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        VX_CORE_ASSERT( pixelsData,"VulkanTexture2D: Failed to load image!");
        m_Width  = texWidth;
        m_Height = texHeight;

        vkclass::VulkanBuffer stagingBuffer = vkclass::VulkanBuffer(pixelsData, imageSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        
        createImage();

        stbi_image_free(pixelsData);
    }

    VulkanTexture2D::~VulkanTexture2D()
    {

    }

    void VulkanTexture2D::Init(VulkanDevice* device)
    {
        s_device = device;
    }

    void VulkanTexture2D::createImage()
{
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = static_cast<uint32_t>(m_Width);
        imageInfo.extent.height = static_cast<uint32_t>(m_Height);
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
    }
}
