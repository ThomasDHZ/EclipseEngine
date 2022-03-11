#pragma once
#include "VulkanRenderer.h"
#include "GraphicsPipeline.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"

class FrameBufferPipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	void SetUpShaderPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
public:
	FrameBufferPipeline();
	FrameBufferPipeline(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
	~FrameBufferPipeline();

	void UpdateGraphicsPipeLine(BuildGraphicsPipelineInfo& buildGraphicsPipelineInfo);
};
