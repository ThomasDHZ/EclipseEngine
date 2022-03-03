#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"

class DrawLinePipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings();
	void SetUpShaderPipeLine(const VkRenderPass& renderPass, VkSampleCountFlagBits sampleCount);
public:
	DrawLinePipeline();
	DrawLinePipeline(const VkRenderPass& renderPass, VkSampleCountFlagBits sampleCount);
	~DrawLinePipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass, VkSampleCountFlagBits sampleCount);
};


