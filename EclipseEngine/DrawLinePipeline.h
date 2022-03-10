#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"

class DrawLinePipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	void SetUpShaderPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
public:
	DrawLinePipeline();
	DrawLinePipeline(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	~DrawLinePipeline();

	void UpdateGraphicsPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
};


