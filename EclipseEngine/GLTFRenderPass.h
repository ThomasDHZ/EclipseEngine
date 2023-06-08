#pragma once
#include "RenderedColorTexture.h"
#include "RenderPass.h"
#include "GLTF_SkyboxPipeline.h"
#include "JsonGraphicsPipeline.h"
class GLTFRenderPass : public RenderPass
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
	GLTFRenderPass();
	~GLTFRenderPass();

	std::shared_ptr<RenderedColorTexture> RenderedTexture;

	void BuildRenderPass(PBRRenderPassTextureSubmitList& textures);
	VkCommandBuffer Draw();
	void Destroy();
};
