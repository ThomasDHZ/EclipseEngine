#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"

class DrawLinePipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	void SetUpShaderPipeLine(const VkRenderPass& renderPass);
public:
	DrawLinePipeline();
	DrawLinePipeline(const VkRenderPass& renderPass, BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	~DrawLinePipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass, BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
};

