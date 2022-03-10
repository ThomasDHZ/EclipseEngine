#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"

class BlinnPhongPipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	void SetUpShaderPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
public:
	BlinnPhongPipeline();
	BlinnPhongPipeline(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	~BlinnPhongPipeline();

	void UpdateGraphicsPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
};

