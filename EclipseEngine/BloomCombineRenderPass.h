#pragma once
#include "RenderPass.h"
#include "BloomCombinePipeline.h"

class BloomCombineRenderPass  : public RenderPass
{
private:
	void RenderPassDesc();
	void BuildRenderPassPipelines(std::vector<std::shared_ptr<RenderedColorTexture>> textureList);

	BloomCombinePipeline bloomPipeline;

public:
	BloomCombineRenderPass();
	~BloomCombineRenderPass();


	std::shared_ptr<RenderedColorTexture> BloomTexture;

	void BuildRenderPass(std::vector<std::shared_ptr<RenderedColorTexture>>textureList);
	void OneTimeDraw(std::vector<std::shared_ptr<RenderedColorTexture>> textureList);
	VkCommandBuffer Draw();
	void Destroy();
};

