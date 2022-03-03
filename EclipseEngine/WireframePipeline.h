#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"

class WireframePipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings();
	void SetUpShaderPipeLine(const VkRenderPass& renderPass, VkSampleCountFlagBits sampleCount);
public:
	WireframePipeline();
	WireframePipeline(const VkRenderPass& renderPass, VkSampleCountFlagBits sampleCount);
	~WireframePipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass, VkSampleCountFlagBits sampleCount);
};