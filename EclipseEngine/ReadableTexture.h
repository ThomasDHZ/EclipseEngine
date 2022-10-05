#pragma once
#include "Texture.h"

class ReadableTexture : public Texture
{
private:
    void CreateTextureImage();
    void CreateTextureView();
    void CreateTextureSampler();
public:
    ReadableTexture();
    ReadableTexture(glm::ivec2 TextureResolution);
    ReadableTexture(glm::ivec2 TextureResolution, VkSampleCountFlagBits sampleCount);
    ReadableTexture(glm::ivec2 TextureResolution, VkSampleCountFlagBits sampleCount, VkFormat format);
    ~ReadableTexture();

    void RecreateRendererTexture(glm::vec2 TextureResolution);
};

