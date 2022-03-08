#pragma once
#include "GraphicsPipeline.h"
#include "GameObjectManager.h"

class WireframePipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings(std::vector<DescriptorSetBindingStruct>& DescriptorBindingList);
	void SetUpShaderPipeLine(const VkRenderPass& renderPass, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits sampleCount);
public:
	WireframePipeline();
	WireframePipeline(const VkRenderPass& renderPass, std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits sampleCount);
	~WireframePipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass, std::vector<DescriptorSetBindingStruct>& DescriptorBindingList, std::vector<VkPipelineColorBlendAttachmentState>& ColorAttachments, VkSampleCountFlagBits sampleCount);
};