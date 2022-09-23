#pragma once
#include "RenderPass.h"
#include "PBRBloomPipeline.h"

class PBRBloomRenderPass : public RenderPass
{
private:
	uint32_t TextureMapMipLevels = 0;

	void RenderPassDesc();
	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);

	std::shared_ptr<RenderedColorTexture> DrawToBloomMap;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;
	PBRBloomPipeline pbrBloomPipeline;

public:
	PBRBloomRenderPass();
	~PBRBloomRenderPass();


	std::shared_ptr<RenderedColorTexture> BloomMap;

	void BuildRenderPass(PBRRenderPassTextureSubmitList& textures);
	void OneTimeDraw(PBRRenderPassTextureSubmitList& textures);
	VkCommandBuffer Draw();
	void Destroy();
};