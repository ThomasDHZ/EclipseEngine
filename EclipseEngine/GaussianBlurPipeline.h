#pragma once
#include <GraphicsPipeline.h>
#include "UniformBuffer.h"
#include "RenderedColorTexture.h"
class GaussianBlurPipeline : public GraphicsPipeline
{

public:
	GaussianBlurPipeline();
	~GaussianBlurPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<std::shared_ptr<RenderedColorTexture>> textureList);
	void Draw(VkCommandBuffer& commandBuffer);
};
