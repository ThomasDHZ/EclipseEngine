#pragma once
#include "GraphicsPipeline.h"

class SkyBoxRenderPipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings();
	void SetUpShaderPipeLine(const VkRenderPass& renderPass, glm::ivec2 RenderPassResolution, VkSampleCountFlagBits SampleCount);
public:
	SkyBoxRenderPipeline();
	SkyBoxRenderPipeline(const VkRenderPass& renderPass, glm::ivec2 RenderPassResolution, VkSampleCountFlagBits SampleCount);
	~SkyBoxRenderPipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass, glm::ivec2 RenderPassResolution, VkSampleCountFlagBits SampleCount);
};