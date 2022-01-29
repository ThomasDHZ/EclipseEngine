#pragma once
#include "Texture.h"

class RenderedTexture : public Texture
{
private:
    void CreateTextureImage();
    void CreateTextureView();
    void CreateTextureSampler();
public:
    RenderedTexture();
    RenderedTexture(glm::ivec2 TextureResolution, RenderedTextureType type);
    RenderedTexture(glm::ivec2 TextureResolution, RenderedTextureType type, VkSampleCountFlagBits sampleCount);
    ~RenderedTexture();

    void RecreateRendererTexture(glm::vec2 TextureResolution);
};