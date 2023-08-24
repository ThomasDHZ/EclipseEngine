#pragma once
#include "RenderPass.h"
#include "JsonGraphicsPipeline.h"


class RenderPass2D : public RenderPass
{
private:
	JsonGraphicsPipeline Renderer2DPipeline;
	//std::vector<JsonGraphicsPipeline> WireframePipelineList;
	JsonGraphicsPipeline LinePipeline;

	void RenderPassDesc();
	void BuildRenderPassPipelines();

public:
	RenderPass2D();
	~RenderPass2D();

	std::shared_ptr<RenderedColorTexture> renderedTexture;
	std::shared_ptr<RenderedDepthTexture> depthTexture;

	void BuildRenderPass();
	VkCommandBuffer Draw(std::vector<std::shared_ptr<GameObject>>& gameObjectList);
	void Destroy();
};

