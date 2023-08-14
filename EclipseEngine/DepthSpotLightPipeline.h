#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "Mesh.h"

class DepthSpotLightPipeline : public GraphicsPipeline
{
public:
	DepthSpotLightPipeline();
	~DepthSpotLightPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<GameObject> mesh, uint32_t x);
};
