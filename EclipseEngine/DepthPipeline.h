#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "Mesh.h"
class DepthPipeline : public GraphicsPipeline
{
public:
	DepthPipeline();
	~DepthPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> mesh, uint32_t x);
};
