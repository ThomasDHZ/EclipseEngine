#pragma once
#include "RenderPass.h"
#include "JsonGraphicsPipeline.h"
#include "Renderer2DPipeline.h"


class RenderPass2D : public RenderPass
{
private:
	std::vector<JsonGraphicsPipeline> Renderer2DPipeline;
	std::vector<JsonGraphicsPipeline> WireframePipelineList;
	std::vector<JsonGraphicsPipeline> LinePipelineList;

	void RenderPassDesc();
	void BuildRenderPassPipelines(std::vector<std::shared_ptr<GameObject>>& gameObjectList);

public:
	RenderPass2D();
	~RenderPass2D();

	std::shared_ptr<RenderedColorTexture> renderedTexture;
	std::shared_ptr<RenderedDepthTexture> depthTexture;

	void BuildRenderPass(std::vector<std::shared_ptr<GameObject>>& gameObjectList);
	VkCommandBuffer Draw(std::vector<std::shared_ptr<GameObject>>& gameObjectList);
	void Destroy();
};

