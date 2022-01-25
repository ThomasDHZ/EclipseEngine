#pragma once
#include "VulkanRenderer.h"
#include "GraphicsPipeline.h"

class FrameBufferPipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings();
	void SetUpShaderPipeLine(const VkRenderPass& renderPass);
public:
	FrameBufferPipeline();
	FrameBufferPipeline(const VkRenderPass& renderPass);
	~FrameBufferPipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass);
};
