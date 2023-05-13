#pragma once
#include "RenderPass.h"
#include "JsonGraphicsPipeline.h"
#include "Renderer2DPipeline.h"


class RenderPass2D : public RenderPass
{
private:
	Renderer2DPipeline oldRenderer2DPipeline;
	JsonGraphicsPipeline Renderer2DPipeline;
	//std::vector<JsonGraphicsPipeline> WireframePipelineList;
	//std::vector<JsonGraphicsPipeline> LinePipelineList;

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

