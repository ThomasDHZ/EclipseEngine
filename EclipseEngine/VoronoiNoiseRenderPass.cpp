#include "VoronoiNoiseRenderPass.h"

VoronoiNoiseRenderPass::VoronoiNoiseRenderPass() : GenerateImageRenderPass()
{
}

VoronoiNoiseRenderPass::~VoronoiNoiseRenderPass()
{
}

void VoronoiNoiseRenderPass::BuildRenderPass(glm::ivec2 textureSize)
{
	GenerateImageRenderPass::BuildRenderPass("/Pipelines/VoronoiNoisePipeline.txt", VK_FORMAT_R8G8B8A8_SRGB, textureSize);
}

void VoronoiNoiseRenderPass::OneTimeDraw(glm::ivec2 textureSize)
{
	GenerateImageRenderPass::OneTimeDraw("/Pipelines/VoronoiNoisePipeline.txt", VK_FORMAT_R8G8B8A8_SRGB, textureSize);
}
