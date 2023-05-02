#pragma once
#include "GraphicsPipeline.h"
#include "SceneManager.h"
#include "RenderedDepthTexture.h"

class Renderer2DPipeline : public GraphicsPipeline
{

public:
	Renderer2DPipeline();
	~Renderer2DPipeline();

	void InitializePipeline(PipelineInfoStruct& pipelineInfoStruct);
	//void Draw(VkCommandBuffer& commandBuffer, std::shared_ptr<Mesh> mesh);
};

