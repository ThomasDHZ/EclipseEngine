#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"

class WireframePipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	void SetUpShaderPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
public:
	WireframePipeline();
	WireframePipeline(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	~WireframePipeline();

	void UpdateGraphicsPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
};