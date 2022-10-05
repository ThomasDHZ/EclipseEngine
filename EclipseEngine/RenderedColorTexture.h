#pragma once
#include "RenderedTexture.h"

class RenderedColorTexture : public RenderedTexture
{
private:
    void CreateTextureImage();
    void CreateTextureView();
    void CreateTextureSampler();
public:
    RenderedColorTexture();
    RenderedColorTexture(glm::ivec2 TextureResolution, VkFormat TextureFormat);
    RenderedColorTexture(glm::ivec2 TextureResolution, VkFormat TextureFormat, VkSampleCountFlagBits sampleCount);
    RenderedColorTexture(glm::ivec2 TextureResolution, VkFormat TextureFormat, VkSampleCountFlagBits sampleCount, uint32_t mipLevels);
    ~RenderedColorTexture();

    void RecreateRendererTexture(glm::vec2 TextureResolution);
    VkAttachmentDescription GetAttachmentDescription();
};


