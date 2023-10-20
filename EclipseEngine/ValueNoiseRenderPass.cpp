#include "ValueNoiseRenderPass.h"

ValueNoiseRenderPass::ValueNoiseRenderPass() : GenerateImageRenderPass()
{
}

ValueNoiseRenderPass::~ValueNoiseRenderPass()
{
}

void ValueNoiseRenderPass::BuildRenderPass(glm::ivec2 textureSize)
{
	GenerateImageRenderPass::BuildRenderPass("/Pipelines/ValueNoisePipeline.txt", VK_FORMAT_R8G8B8A8_SRGB, textureSize);
}

void ValueNoiseRenderPass::OneTimeDraw(glm::ivec2 textureSize)
{
	GenerateImageRenderPass::OneTimeDraw("/Pipelines/ValueNoisePipeline.txt", VK_FORMAT_R8G8B8A8_SRGB, textureSize);
}
