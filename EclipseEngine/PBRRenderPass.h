#pragma once
#include "RenderedColorTexture.h"
#include "RenderPass.h"
#include "JsonGraphicsPipeline.h"
class PBRRenderPass : public RenderPass
{
private:
	std::shared_ptr<RenderedColorTexture> ColorTexture;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	JsonGraphicsPipeline PBRPipeline;
	JsonGraphicsPipeline PBRInstancePipeline;
	JsonGraphicsPipeline WireframePipeline;
	JsonGraphicsPipeline WireframeInstancePipeline;
	JsonGraphicsPipeline LinePipeline;
	JsonGraphicsPipeline SkyBoxPipeline;
	JsonGraphicsPipeline lightReflectionPipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);

public:
	PBRRenderPass();
	~PBRRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void BuildRenderPass(PBRRenderPassTextureSubmitList& textures);
	VkCommandBuffer Draw();
	void Destroy();
};
