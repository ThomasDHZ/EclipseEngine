#include "BRDFRenderPass.h"

BRDFRenderPass::BRDFRenderPass() : GenerateImageRenderPass()
{
}

BRDFRenderPass::~BRDFRenderPass()
{
}

void BRDFRenderPass::BuildRenderPass(uint32_t textureSize)
{
	GenerateImageRenderPass::BuildRenderPass("/Pipelines/BRDFPipeline.txt", glm::ivec2(textureSize));
}

void BRDFRenderPass::OneTimeDraw(uint32_t textureSize)
{
	GenerateImageRenderPass::OneTimeDraw("/Pipelines/BRDFPipeline.txt", glm::ivec2(textureSize));
}
