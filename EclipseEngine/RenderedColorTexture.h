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
    RenderedColorTexture(glm::ivec2 TextureResolution, VkFormat TextureFormat);
    RenderedColorTexture(glm::ivec2 TextureResolution, VkFormat TextureFormat, VkSampleCountFlagBits sampleCount);
    ~RenderedColorTexture();

    void RecreateRendererTexture(glm::vec2 TextureResolution);
    VkAttachmentDescription GetAttachmentDescription();
};


