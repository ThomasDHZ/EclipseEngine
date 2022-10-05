#pragma once
#include "Texture.h"
#include "stb_image_write.h"
#include <fstream>

enum BakeTextureFormat
{
    Bake_BMP,
    Bake_JPG,
    Bake_PNG,
    Bake_TGA
};

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
    RenderedTexture(glm::ivec2 TextureResolution, VkSampleCountFlagBits sampleCount, VkFormat format);
    ~RenderedTexture();

    void RecreateRendererTexture(glm::vec2 TextureResolution);
    void BakeDepthTexture(const char* filename, BakeTextureFormat textureFormat);
    void BakeColorTexture(const char* filename, BakeTextureFormat textureFormat);
    void BakeEnvironmentMapTexture(const char* filename);
};