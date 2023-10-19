#include "MultiplyRenderPass.h"

MultiplyRenderPass::MultiplyRenderPass()
{
}

MultiplyRenderPass::~MultiplyRenderPass()
{
}

void MultiplyRenderPass::BuildRenderPass(glm::ivec2 textureSize, std::shared_ptr<Texture> texture1, std::shared_ptr<Texture> texture2)
{
	GenerateImageRenderPass::BuildRenderPass("/Pipelines/MultiplyTexturePipeline.txt", textureSize, texture1, texture2);
}

void MultiplyRenderPass::OneTimeDraw(glm::ivec2 textureSize, std::shared_ptr<Texture> texture1, std::shared_ptr<Texture> texture2)
{
	GenerateImageRenderPass::BuildRenderPass("/Pipelines/MultiplyTexturePipeline.txt", textureSize, texture1, texture2);
}
