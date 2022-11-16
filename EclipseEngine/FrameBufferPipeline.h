#pragma once
#include <GraphicsPipeline.h>
#include "SceneManager.h"
#include "Mesh.h"

class FrameBufferPipeline : public GraphicsPipeline
{
public:
	FrameBufferPipeline();
	~FrameBufferPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedColorTexture> renderedTexture);
	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct, std::shared_ptr<RenderedColorTexture> renderedTexture, std::shared_ptr<RenderedColorTexture> bloomTexture);
	void Draw(VkCommandBuffer& commandBuffer);
};
