#pragma once
#include "VulkanRenderer.h"
#include "GraphicsPipeline.h"
#include "RenderedTexture.h"

class FrameBufferPipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings(std::shared_ptr<RenderedTexture> RenderedTexture);
	void SetUpShaderPipeLine(const VkRenderPass& renderPass, std::shared_ptr<RenderedTexture> RenderedTexture);
public:
	FrameBufferPipeline();
	FrameBufferPipeline(const VkRenderPass& renderPass, std::shared_ptr<RenderedTexture> RenderedTexture);
	~FrameBufferPipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass, std::shared_ptr<RenderedTexture> RenderedTexture);
};
