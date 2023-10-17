#include "PerlinNoiseRenderPass.h"

PerlinNoiseRenderPass::PerlinNoiseRenderPass() : GenerateImageRenderPass()
{
}

PerlinNoiseRenderPass::~PerlinNoiseRenderPass()
{
}

void PerlinNoiseRenderPass::BuildRenderPass(glm::ivec2 textureSize)
{
	GenerateImageRenderPass::BuildRenderPass("PerlinNoisePipeline.txt", textureSize);
}

void PerlinNoiseRenderPass::OneTimeDraw(glm::ivec2 textureSize)
{
	GenerateImageRenderPass::OneTimeDraw("PerlinNoisePipeline.txt", textureSize);
}
