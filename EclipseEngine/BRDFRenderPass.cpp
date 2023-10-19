#include "BRDFRenderPass.h"

BRDFRenderPass::BRDFRenderPass() : GenerateImageRenderPass()
{
}

BRDFRenderPass::~BRDFRenderPass()
{
}

void BRDFRenderPass::BuildRenderPass(uint32_t textureSize)
{
	GenerateImageRenderPass::BuildRenderPass("/Pipelines/BRDFPipeline.txt", VK_FORMAT_R8G8B8A8_UNORM, glm::ivec2(textureSize));
}

void BRDFRenderPass::OneTimeDraw(uint32_t textureSize)
{
	GenerateImageRenderPass::OneTimeDraw("/Pipelines/BRDFPipeline.txt", VK_FORMAT_R8G8B8A8_UNORM, glm::ivec2(textureSize));
}
