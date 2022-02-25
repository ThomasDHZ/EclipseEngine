#pragma once
#include "Texture.h"
class Texture2D : public Texture
{
protected:
    void CreateTextureView(VkFormat format);
    void CreateTextureSampler();
public:
    Texture2D();
    Texture2D(nlohmann::json& json);
    Texture2D(const std::string TextureLocation, TextureTypeEnum textureType, VkFormat format);
    ~Texture2D();
};