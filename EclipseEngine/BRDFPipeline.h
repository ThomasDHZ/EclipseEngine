#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "Mesh.h"
class BRDFPipeline : public GraphicsPipeline
{
public:
	BRDFPipeline();
	~BRDFPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	void Draw(VkCommandBuffer& commandBuffer);
};
