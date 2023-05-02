#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class LinePipeline : public GraphicsPipeline
{
public:
	LinePipeline();
	~LinePipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	//void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh);
};

