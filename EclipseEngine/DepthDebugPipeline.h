#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "Mesh.h"
#include "RenderedDepthTexture.h"

class DepthDebugPipeline : public GraphicsPipeline
{
public:
	DepthDebugPipeline();
	~DepthDebugPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedDepthTexture> depthTexture);
	void Draw(VkCommandBuffer& commandBuffer);
};

