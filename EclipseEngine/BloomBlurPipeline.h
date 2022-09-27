#pragma once
#include "GraphicsPipeline.h"
#include "UniformBuffer.h"
#include "RenderedColorTexture.h"
class BloomBlurPipeline : public GraphicsPipeline
{

public:
	BloomBlurPipeline();
	~BloomBlurPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<std::shared_ptr<RenderedColorTexture>> textureList);
	void Draw(VkCommandBuffer& commandBuffer, BloomSettings& bloomSettings);
};
