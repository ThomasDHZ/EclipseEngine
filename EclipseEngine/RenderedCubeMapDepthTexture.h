#pragma once
#include "CubeMapTexture.h"

class RenderedCubeMapDepthTexture : public CubeMapTexture
{
private:
    void CreateTextureImage();
    void CreateTextureView();
    void CreateTextureSampler();
public:
    RenderedCubeMapDepthTexture();
    RenderedCubeMapDepthTexture(glm::ivec2 TextureResolution, VkSampleCountFlagBits sampleCount, uint32_t mipLevels = 1);
    ~RenderedCubeMapDepthTexture();

    void RecreateRendererTexture(glm::ivec2 TextureResolution);
    VkAttachmentDescription GetAttachmentDescription();
};
