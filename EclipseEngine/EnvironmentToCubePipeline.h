#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class EnvironmentToCubePipeline : public GraphicsPipeline
{
public:
	EnvironmentToCubePipeline();
	~EnvironmentToCubePipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	void Draw(VkCommandBuffer& commandBuffer);
};