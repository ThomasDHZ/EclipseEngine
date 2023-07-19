#pragma once
#include "RenderedColorTexture.h"
#include "RenderPass.h"
#include "JsonGraphicsPipeline.h"
class PBRRenderPass : public RenderPass
{
private:
	std::shared_ptr<RenderedColorTexture> ColorTexture;


	JsonGraphicsPipeline PBRPipeline;
	JsonGraphicsPipeline PBRInstancePipeline;
	JsonGraphicsPipeline WireframePipeline;
	JsonGraphicsPipeline WireframeInstancePipeline;
	JsonGraphicsPipeline LinePipeline;
	JsonGraphicsPipeline SkyBoxPipeline;
	JsonGraphicsPipeline lightReflectionPipeline;

	void BuildRenderPassPipelines(PBRRenderPassTextureSubmitList& textures);

public:
	PBRRenderPass();
	~PBRRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;
	std::shared_ptr<RenderedDepthTexture> DepthTexture;

	void RenderPassDesc();
	void BuildRenderPass(std::string& renderPass, PBRRenderPassTextureSubmitList& textures);
	VkCommandBuffer Draw();
	void Destroy();
};
