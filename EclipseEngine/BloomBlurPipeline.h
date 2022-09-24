#pragma once
#include "GraphicsPipeline.h"
#include "UniformBuffer.h"
class BloomBlurPipeline : public GraphicsPipeline
{

public:
	BloomBlurPipeline();
	~BloomBlurPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<Texture> texture);
	void Draw(VkCommandBuffer& commandBuffer, BloomSettings& bloomSettings);
};
