#pragma once
#include "Texture.h"
class Texture2D : public Texture
{
protected:
    void CreateTextureView(VkFormat format);
    void CreateTextureSampler();
public:
    Texture2D();
    Texture2D(const std::string TextureLocation, VkFormat format);
    ~Texture2D();
};