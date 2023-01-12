#pragma once
#include "Texture.h"
#include <fstream>

class BakedTexture : public Texture
{
private:
    void CreateTextureImage();
    void CreateTextureView();
    void CreateTextureSampler();
public:
    BakedTexture();
    BakedTexture(const glm::ivec2& TextureResolution);
    BakedTexture(const Pixel& ClearColor, const glm::ivec2& TextureResolution);
    ~BakedTexture();

    void RecreateRendererTexture(glm::vec2 TextureResolution);
};
