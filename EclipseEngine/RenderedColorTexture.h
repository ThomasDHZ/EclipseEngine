#pragma once
#include "Texture.h"

class RenderedColorTexture : public Texture
{
private:
    void CreateTextureImage();
    void CreateTextureView();
    void CreateTextureSampler();
public:
    RenderedColorTexture();
    RenderedColorTexture(glm::ivec2 TextureResolution);
    RenderedColorTexture(glm::ivec2 TextureResolution, VkSampleCountFlagBits sampleCount);
    ~RenderedColorTexture();

    void RecreateRendererTexture(glm::vec2 TextureResolution);
};

