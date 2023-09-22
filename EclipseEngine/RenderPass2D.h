#pragma once
#include "RenderPass.h"
#include "JsonGraphicsPipeline.h"


class RenderPass2D : public RenderPass
{
private:
	JsonGraphicsPipeline LevelRenderer2DPipeline;
	JsonGraphicsPipeline Renderer2DPipeline;
	JsonGraphicsPipeline WireframePipeline;
	JsonGraphicsPipeline LinePipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	RenderPass2D();
	~RenderPass2D();

	std::shared_ptr<RenderedColorTexture> renderedTexture;
	std::shared_ptr<RenderedColorTexture> bloomTexture;
	std::shared_ptr<RenderedDepthTexture> depthTexture;

	void BuildRenderPass();
	VkCommandBuffer Draw(std::vector<GameObject2D*> gameObjectList);
	void Destroy();
};

