#pragma once
#include "Texture.h"
class Texture2D : public Texture
{
protected:
    void CreateTextureView(VkFormat format);
    void CreateTextureSampler();
public:
    Texture2D();
    //Texture2D(nlohmann::json& json);
    Texture2D(const Pixel& ClearColor, const glm::ivec2& Resolution, VkFormat format, TextureTypeEnum textureType);
    Texture2D(const std::string TextureLocation, TextureTypeEnum textureType, VkFormat format);
    Texture2D(const TinyGltfTextureLoader& textureLoader, const TinyGltfTextureSamplerLoader& samplerLoader, VkFormat format, TextureTypeEnum textureType);
    ~Texture2D();

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Texture2D, FilePath, TextureName, Width, Height, Depth, MipMapLevels, SampleCount, StartTextureByteFormat, TextureByteFormat, TextureImageLayout)
};