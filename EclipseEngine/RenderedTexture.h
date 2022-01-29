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
    RenderedTexture(glm::ivec2 TextureResolution);
    RenderedTexture(glm::ivec2 TextureResolution, VkSampleCountFlagBits sampleCount);
    ~RenderedTexture();

    void RecreateRendererTexture(glm::vec2 TextureResolution);
};