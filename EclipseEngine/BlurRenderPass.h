#pragma once
#include "RenderPass.h"
#include "BloomBlurPipeline.h"

class BlurRenderPass : public RenderPass
{
private:
	uint32_t TextureMapMipLevels = 0;

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::vector<std::shared_ptr<RenderedColorTexture>> textureList);

	std::shared_ptr<RenderedColorTexture> DrawToBloomMap;
	BloomBlurPipeline blurPipeline;

public:
	BlurRenderPass();
	~BlurRenderPass();


	std::vector<std::shared_ptr<RenderedColorTexture>> BlurredTextureList;

	void BuildRenderPass(std::vector<std::shared_ptr<RenderedColorTexture>>textureList);
	void OneTimeDraw(std::vector<std::shared_ptr<RenderedColorTexture>> textureList, float horizontalpass);
	VkCommandBuffer Draw(float horizontalpass);
	void Destroy();
};