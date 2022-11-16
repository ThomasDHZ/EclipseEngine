#pragma once
#include <GraphicsPipeline.h>
#include "SceneManager.h"
#include "Mesh.h"

class DepthInstancedPipeline : public GraphicsPipeline
{
public:
	DepthInstancedPipeline();
	~DepthInstancedPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh, uint32_t x);
};
