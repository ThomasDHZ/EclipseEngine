#pragma once
#include "Texture.h"

enum RenderedTextureType
{
    RenderedColorTexture,
    RenderedDepthTexture
};

class RenderedTexture : public Texture
{
private:
    RenderedTextureType renderedTexture;
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