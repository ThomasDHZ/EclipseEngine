#pragma once
#include "RenderPass.h"
#include "BloomBlurPipeline.h"

class BlurRenderPass : public RenderPass
{
private:
	uint32_t TextureMapMipLevels = 0;

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::shared_ptr<Texture> texture);

	std::shared_ptr<RenderedColorTexture> DrawToBloomMap;
	BloomBlurPipeline blurPipeline;

public:
	BlurRenderPass();
	~BlurRenderPass();


	std::shared_ptr<RenderedColorTexture> BlurredTexture;

	void BuildRenderPass(std::shared_ptr<Texture> texture);
	void OneTimeDraw(std::shared_ptr<Texture> texture);
	VkCommandBuffer Draw();
	void Destroy();
};