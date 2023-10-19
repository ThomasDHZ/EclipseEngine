#include "PerlinNoiseRenderPass.h"

PerlinNoiseRenderPass::PerlinNoiseRenderPass() : GenerateImageRenderPass()
{
}

PerlinNoiseRenderPass::~PerlinNoiseRenderPass()
{
}

void PerlinNoiseRenderPass::BuildRenderPass(glm::ivec2 textureSize)
{
	GenerateImageRenderPass::BuildRenderPass("/Pipelines/PerlinNoisePipeline.txt", VK_FORMAT_R8G8B8A8_SRGB, textureSize);
}

void PerlinNoiseRenderPass::OneTimeDraw(glm::ivec2 textureSize)
{
	GenerateImageRenderPass::OneTimeDraw("/Pipelines/PerlinNoisePipeline.txt", VK_FORMAT_R8G8B8A8_SRGB, textureSize);
}
