#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"

class WireFramePipeline : public GraphicsPipeline
{
public:
	WireFramePipeline();
	~WireFramePipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	//void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh);
};

