#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "Mesh.h"

class FrameBufferPipeline : public GraphicsPipeline
{
public:
	FrameBufferPipeline();
	~FrameBufferPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedColorTexture> renderedTexture);
	void Draw(VkCommandBuffer& commandBuffer);
};
