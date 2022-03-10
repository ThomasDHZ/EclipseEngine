#pragma once
#include "GraphicsPipeline.h"
#include "UniformBuffer.h"
#include "GameObjectManager.h"
#include "Texture2D.h"

class Renderer2DPipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	void SetUpShaderPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
public:

	Renderer2DPipeline();
	Renderer2DPipeline(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	~Renderer2DPipeline();

	void UpdateGraphicsPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
};

