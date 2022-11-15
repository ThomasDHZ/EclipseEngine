#pragma once
#include "VulkanRenderer.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "stb_image.h"
#include "JsonConverter.h"
#include "Pixel.h"

enum TextureTypeEnum
{
    kUndefinedTexture,
    kTextureAtlus,
    kRenderedColorTexture,
    kRenderedDepthTexture,
    kReadableTexture,
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
    kCubeMapTexture,
    kCubeMapDepthTexture,
    kEnvironmentTexture,
    kRenderedCubeMap,
    kBakedTexture
};

struct TextureLoader
{
    Pixel ClearColor;
    int Width;
    int Height;
    int Depth;
    VkFormat TextureFormat;
    TextureTypeEnum TextureType;
};

class Texture
{
private:
    static uint64_t TextureIDCounter;
    uint64_t TextureID = 0;
    uint64_t TextureBufferIndex = 0;

    void LoadTexture(const Pixel& ClearColor, const glm::ivec2& Resolution, VkFormat format);
    void LoadTexture(const std::vector<Pixel>& pixels, const glm::ivec2& Resolution, VkFormat format);
    void LoadTexture(const std::vector<glm::vec4>& pixels, const glm::ivec2& Resolution, VkFormat format);
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

    void GenerateID();
    void CreateTextureImage(VkImageCreateInfo TextureInfo);
    void TransitionImageLayout(VkImageLayout newImageLayout);
    void CopyBufferToImage(VkBuffer buffer);

public:
    Texture();
    Texture(const Pixel& ClearColor, const glm::ivec2& Resolution, VkFormat format, TextureTypeEnum textureType);
    Texture(const std::vector<Pixel> pixels, const glm::ivec2& Resolution, TextureTypeEnum textureType);
    Texture(const std::vector<glm::vec4> pixels, const glm::ivec2& Resolution, TextureTypeEnum textureType);
    Texture(TextureTypeEnum textureType);
    Texture(nlohmann::json& json);
    Texture(std::string TextureLocation, TextureTypeEnum textureType, VkFormat format);

    ~Texture();

    void UpdateImageLayout(VkImageLayout newImageLayout);
    void UpdateImageLayout(VkImageLayout newImageLayout, uint32_t MipLevel);
    void UpdateImageLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout);
    void UpdateImageLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout, uint32_t MipLevel);
    void UpdateImageLayout(VkCommandBuffer& commandBuffer, VkImageLayout oldImageLayout, VkImageLayout newImageLayout);
    void UpdateImageLayout(VkCommandBuffer& commandBuffer, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, uint32_t MipLevel);

    void UpdateCubeMapLayout(VkImageLayout newImageLayout);
    void UpdateCubeMapLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout);
    void UpdateCubeMapLayout(VkImageLayout newImageLayout, uint32_t MipLevel);
    void UpdateCubeMapLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout, uint32_t MipLevel);

    void UpdateDepthImageLayout(VkImageLayout newImageLayout);
    void UpdateDepthImageLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout);
    void UpdateDepthImageLayout(VkCommandBuffer& commandBuffer, VkImageLayout oldImageLayout, VkImageLayout newImageLayout);

    void UpdateDepthCubeMapLayout(VkImageLayout newImageLayout);
    void UpdateDepthCubeMapLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout);
    void UpdateDepthCubeMapLayout(VkImageLayout newImageLayout, uint32_t MipLevel);
    void UpdateDepthCubeMapLayout(VkCommandBuffer& commandBuffer, VkImageLayout newImageLayout, uint32_t MipLevel);

    void Destroy();

    void SetTextureBufferIndex(uint64_t bufferIndex);

    static void CopyTexture(VkCommandBuffer& commandBuffer, Texture* srcTexture, Texture* dstTexture);
    static void CopyTexture(VkCommandBuffer& commandBuffer, Texture* srcTexture, Texture* dstTexture, uint32_t MipLevel);
    static void CopyTexture(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture);
    static void CopyTexture(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture, uint32_t MipLevel);

    static void CopyMipLevelToTexture(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture);

    static void CopyDepthTexture(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture);

    static void CopyCubeMap(std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture);
    static void CopyCubeMap(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture);
    static void CopyCubeMap(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture, uint32_t MipLevel);

    static void CopyDepthCubeMap(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture);
    static void CopyDepthCubeMap(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture, uint32_t MipLevel);

    static void CopyMipLevelToCubeMap(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture);
    static void CopyCubeMapLayer(VkCommandBuffer& commandBuffer, std::shared_ptr<Texture> srcTexture, std::shared_ptr<Texture> dstTexture, uint32_t layer, uint32_t MipLevel);

    void ImGuiShowTexture(const ImVec2& TextureDisplaySize);

    VkDescriptorSet ImGuiDescriptorSet = VK_NULL_HANDLE;
    VkImage Image = VK_NULL_HANDLE;
    VkDeviceMemory Memory = VK_NULL_HANDLE;
    VkImageView View = VK_NULL_HANDLE;
    VkSampler Sampler = VK_NULL_HANDLE;

    std::string GetFilePath() { return FilePath; }
    std::string GetTextureName() { return TextureName; }
    uint64_t GetTextureID() { return TextureID; }
    VkImageView GetView() { return View; }
    VkSampler GetSampler() { return Sampler; }
    VkImageLayout GetImageLayout() { return TextureImageLayout; }
    VkImageView* GetViewPtr() { return &View; }
    VkSampler* GetSamplerPtr() { return &Sampler; }
    uint64_t GetTextureBufferIndex() { return TextureBufferIndex; }
    uint32_t GetMipLevels() { return MipMapLevels; }
    int GetWidth() { return Width; }
    int GetHeight() { return Height; }
    int GetDepth() { return Depth; }


    void to_json(nlohmann::json& json)
    {
        JsonConverter::to_json(json["FilePath"], FilePath);
        JsonConverter::to_json(json["TextureName"], TextureName);
        JsonConverter::to_json(json["Width"], Width);
        JsonConverter::to_json(json["Height"], Height);
        JsonConverter::to_json(json["Depth"], Depth);
        JsonConverter::to_json(json["MipMapLevels"], MipMapLevels);
        JsonConverter::to_json(json["SampleCount"], SampleCount);
        JsonConverter::to_json(json["TextureType"], TextureType);
        JsonConverter::to_json(json["StartTextureByteFormat"], StartTextureByteFormat);
        JsonConverter::to_json(json["TextureByteFormat"], TextureByteFormat);
        JsonConverter::to_json(json["TextureImageLayout"], TextureImageLayout);
    }

};

