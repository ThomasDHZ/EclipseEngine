#pragma once
#include "VulkanRenderer.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "stb_image.h"
#include <json.hpp>

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
    VkFormat TextureByteFormat = VK_FORMAT_UNDEFINED;
    VkImageLayout TextureImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkSampleCountFlagBits SampleCount = VK_SAMPLE_COUNT_1_BIT;

    VkImage Image = VK_NULL_HANDLE;
    VkDeviceMemory Memory = VK_NULL_HANDLE;

    void GenerateID();
    void CreateTextureImage(VkImageCreateInfo TextureInfo);

public:
    Texture();
    Texture(std::string TextureLocation, VkFormat format);

    ~Texture();

    void UpdateImageLayout(VkImageLayout newImageLayout);
    void Destroy();

    void SetTextureBufferIndex(uint64_t bufferIndex);

    VkDescriptorSet ImGuiDescriptorSet = VK_NULL_HANDLE;
    VkImageView View = VK_NULL_HANDLE;
    VkSampler Sampler = VK_NULL_HANDLE;


    std::string GetFilePath() { return FilePath; }
    uint64_t GetTextureID() { return TextureID; }
    VkImageView GetView() { return View; }
    VkSampler GetSampler() { return Sampler; }
    VkImageView* GetViewPtr() { return &View; }
    VkSampler* GetSamplerPtr() { return &Sampler; }
    uint64_t GetTextureBufferIndex() { return TextureBufferIndex; }

    void to_json(nlohmann::json& j, const Texture& p)
    {
        j["Width"] = Width;
        j["Height"] = Height;
        j["Depth"] = Depth;
        j["TextureByteFormat"] = TextureByteFormat;
        j["TextureImageLayout"] = TextureImageLayout;
        j["SampleCount"] = SampleCount;
    }

    void from_json(const nlohmann::json& j, Texture& p)
    {
        j.at("Width").get_to(p.Width);
        j.at("Height").get_to(p.Height);
        j.at("Depth").get_to(p.Depth);
        j.at("MipMapLevels").get_to(p.MipMapLevels);
        j.at("TextureByteFormat").get_to(p.TextureByteFormat);
        j.at("TextureImageLayout").get_to(p.TextureImageLayout);
        j.at("SampleCount").get_to(p.SampleCount);
    }
};

