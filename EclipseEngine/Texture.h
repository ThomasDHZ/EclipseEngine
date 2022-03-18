#pragma once
#include "VulkanRenderer.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "stb_image.h"
#include <json.hpp>

enum TextureTypeEnum
{
    kUndefinedTexture,
    kRenderedColorTexture,
    kRenderedDepthTexture,
    kDiffuseTextureMap,
    kSpecularTextureMap,
    kAlbedoTextureMap,
    kMetallicTextureMap,
    kRoughnessTextureMap,
    kAmbientOcclusionTextureMap,
    kNormalTextureMap,
    kDepthTextureMap,
    kAlphaTextureMap,
    kEmissionTextureMap,
};

class Texture
{
private:
    static uint64_t TextureIDCounter;
    uint64_t TextureID = 0;
    uint64_t TextureBufferIndex = 0;

    void TransitionImageLayout(VkImageLayout newImageLayout);
    void CopyBufferToImage(VkBuffer buffer);
    void LoadTexture(std::string TextureLocation, VkFormat format);
    void GenerateMipmaps();

protected:
    std::string FilePath;
    std::string TextureName;

    int Width;
    int Height;
    int Depth;
    uint32_t MipMapLevels = 1;

    TextureTypeEnum TextureType = kUndefinedTexture;
    VkFormat StartTextureByteFormat = VK_FORMAT_UNDEFINED;
    VkFormat TextureByteFormat = VK_FORMAT_UNDEFINED;
    VkImageLayout TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkSampleCountFlagBits SampleCount = VK_SAMPLE_COUNT_1_BIT;

    VkImage Image = VK_NULL_HANDLE;
    VkDeviceMemory Memory = VK_NULL_HANDLE;

    void GenerateID();
    void CreateTextureImage(VkImageCreateInfo TextureInfo);

public:
    Texture();
    Texture(TextureTypeEnum textureType);
    Texture(nlohmann::json& json);
    Texture(std::string TextureLocation, TextureTypeEnum textureType, VkFormat format);

    ~Texture();

    void UpdateImageLayout(VkImageLayout newImageLayout);
    void UpdateImageLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout);
    void Destroy();

    void SetTextureBufferIndex(uint64_t bufferIndex);

    VkDescriptorSet ImGuiDescriptorSet = VK_NULL_HANDLE;
    VkImageView View = VK_NULL_HANDLE;
    VkSampler Sampler = VK_NULL_HANDLE;

    std::string GetFilePath() { return FilePath; }
    std::string GetTextureName() { return TextureName; }
    uint64_t GetTextureID() { return TextureID; }
    VkImageView GetView() { return View; }
    VkSampler GetSampler() { return Sampler; }
    VkImageView* GetViewPtr() { return &View; }
    VkSampler* GetSamplerPtr() { return &Sampler; }
    uint64_t GetTextureBufferIndex() { return TextureBufferIndex; }

    virtual nlohmann::json ToJson()
    {
        nlohmann::json json;

        json["FilePath"] = FilePath;
        json["TextureName"] = TextureName;

        json["Width"] = Width;
        json["Height"] = Height;
        json["Depth"] = Depth;
        json["MipMapLevels"] = MipMapLevels;
        json["SampleCount"] = SampleCount;

        json["TextureType"] = TextureType;
        json["StartTextureByteFormat"] = StartTextureByteFormat;
        json["TextureByteFormat"] = TextureByteFormat;
        json["TextureImageLayout"] = TextureImageLayout;

        return json;
    }

};

