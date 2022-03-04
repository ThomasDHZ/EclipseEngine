#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"

class DrawLinePipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings();
	void SetUpShaderPipeLine(const VkRenderPass& renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits sampleCount);
public:
	DrawLinePipeline();
	DrawLinePipeline(const VkRenderPass& renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits sampleCount);
	~DrawLinePipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits sampleCount);
};


