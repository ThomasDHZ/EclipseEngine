#pragma once
#include "VulkanRenderer.h"
#include "GraphicsPipeline.h"
#include "RenderedTexture.h"
#include "RenderedColorTexture.h"

class FrameBufferPipeline : public GraphicsPipeline
{
private:
	void SetUpDescriptorBindings(std::shared_ptr<RenderedColorTexture> RenderedTexture);
	void SetUpShaderPipeLine(const VkRenderPass& renderPass, std::shared_ptr<RenderedColorTexture> RenderedTexture);
public:
	FrameBufferPipeline();
	FrameBufferPipeline(const VkRenderPass& renderPass, std::shared_ptr<RenderedColorTexture> RenderedTexture);
	~FrameBufferPipeline();

	void UpdateGraphicsPipeLine(const VkRenderPass& renderPass, std::shared_ptr<RenderedColorTexture> RenderedTexture);
};
