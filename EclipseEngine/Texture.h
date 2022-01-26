#pragma once
#include "VulkanRenderer.h"
#include "stb_image.h"

class Texture
{
private:
    int Width;
    int Height;
    int Depth;
    uint32_t MipMapLevels = 1;

    VkImage Image = VK_NULL_HANDLE;
    VkImageView View = VK_NULL_HANDLE;
    VkSampler Sampler = VK_NULL_HANDLE;
    VkDeviceMemory Memory = VK_NULL_HANDLE;
    VkDescriptorSet ImGuiDescriptorSet = VK_NULL_HANDLE;
    VkFormat TextureByteFormat = VK_FORMAT_UNDEFINED;
    VkImageLayout TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;


    void CopyBufferToImage(VkBuffer buffer);
    void LoadTexture(std::string TextureLocation, VkFormat format);
    void CreateTextureImage(VkImageCreateInfo TextureInfo);
    void GenerateMipmaps();
  
public:
    Texture();
    Texture(std::string TextureLocation, VkFormat format);
    ~Texture();

    void UpdateImageLayout(VkImageLayout newImageLayout);
};

