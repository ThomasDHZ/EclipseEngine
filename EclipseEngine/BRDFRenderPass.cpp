#include "BRDFRenderPass.h"

BRDFRenderPass::BRDFRenderPass() : GenerateImageRenderPass()
{
}

BRDFRenderPass::~BRDFRenderPass()
{
}

void BRDFRenderPass::BuildRenderPass(uint32_t textureSize)
{
	GenerateImageRenderPass::BuildRenderPass("BRDFPipeline.txt", glm::ivec2(textureSize));
}

void BRDFRenderPass::OneTimeDraw(uint32_t textureSize)
{
	GenerateImageRenderPass::OneTimeDraw("BRDFPipeline.txt", glm::ivec2(textureSize));
}
