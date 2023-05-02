#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"

class LightDebugPipeline : public GraphicsPipeline
{
public:
	LightDebugPipeline();
	~LightDebugPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	//void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh);
};

