#pragma once
#include "RenderPass.h"
#include "GaussianBlurPipeline.h"

class GaussianBlurRenderPass : public RenderPass
{
private:
	uint32_t TextureMapMipLevels = 0;

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::vector<std::shared_ptr<RenderedColorTexture>> textureList);

	std::shared_ptr<RenderedColorTexture> DrawToBloomMap;
	GaussianBlurPipeline blurPipeline;

public:
	GaussianBlurRenderPass();
	~GaussianBlurRenderPass();

	std::vector<std::shared_ptr<RenderedColorTexture>> BlurredTextureList;

	void BuildRenderPass(std::vector<std::shared_ptr<RenderedColorTexture>>textureList);
	void OneTimeDraw(std::vector<std::shared_ptr<RenderedColorTexture>> textureList);
	VkCommandBuffer Draw();
	void Destroy();
};