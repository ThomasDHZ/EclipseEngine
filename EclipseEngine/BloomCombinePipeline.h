#pragma once
#include <GraphicsPipeline.h>
#include "UniformBuffer.h"
#include "RenderedColorTexture.h"

class BloomCombinePipeline : public GraphicsPipeline
{
private:
	BloomIndexSettings bloomTextureCount;

public:
	BloomCombinePipeline();
	~BloomCombinePipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::vector<std::shared_ptr<RenderedColorTexture>> textureList);
	void Draw(VkCommandBuffer& commandBuffer);
};
